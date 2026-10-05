#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <string>

#include "attendance/absence.hpp"
#include "attendance/sick_leave.hpp"
#include "attendance/vacation.hpp"
#include "company.hpp"
#include "departments/hr_department.hpp"
#include "departments/it_department.hpp"
#include "exceptions/department_not_found_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "exceptions/overtime_limit_exceeded_exception.hpp"
#include "workers/worker.hpp"

#include "test_helpers.hpp"

namespace {

constexpr double kTolerance = 0.001;

void populateCompanyWithDeveloper(Company& company, int& developerId) {
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);

    Worker* developer =
        company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, 160000.0);
    developerId = developer->getId();
}

}  // namespace

TEST(AbsenceTest_RejectsInvalidDays) {
    CHECK_THROW(Absence(0, Absence::kFullPay), InvalidInputException);
    CHECK_THROW(Absence(-3, Absence::kFullPay), InvalidInputException);
    CHECK_THROW(Absence::dayOff(0), InvalidInputException);
}

TEST(AbsenceTest_DayOffIsPaidPartially) {
    const std::unique_ptr<Absence> dayOff = Absence::dayOff(2);

    CHECK(dayOff != nullptr);
    CHECK_EQUAL(2, dayOff->getDays());
    CHECK_CLOSE(Absence::kHalfPay, dayOff->getPayRate(), kTolerance);
}

TEST(VacationTest_PaidAndUnpaidDifferOnlyByRate) {
    const std::unique_ptr<Vacation> paid = Vacation::paid(14);
    const std::unique_ptr<Vacation> unpaid = Vacation::unpaid(14);

    CHECK(paid != nullptr);
    CHECK(unpaid != nullptr);
    CHECK_EQUAL(paid->getDays(), unpaid->getDays());
    CHECK_CLOSE(Absence::kFullPay, paid->getPayRate(), kTolerance);
    CHECK_CLOSE(Absence::kNoPay, unpaid->getPayRate(), kTolerance);
    CHECK(paid->getTypeName() != unpaid->getTypeName());
}

TEST(VacationTest_IsUsableThroughBaseReference) {
    const Vacation paid(5, true);
    const Vacation unpaid(5, false);
    const Absence& asPaid = paid;
    const Absence& asUnpaid = unpaid;

    CHECK_CLOSE(1.0, asPaid.getPayRate(), kTolerance);
    CHECK_CLOSE(0.0, asUnpaid.getPayRate(), kTolerance);
}

TEST(SickLeaveTest_PayRateDependsOnSeniority) {
    CHECK_CLOSE(0.6, SickLeave::forDays(7, 0)->getPayRate(), kTolerance);
    CHECK_CLOSE(0.6, SickLeave::forDays(7, 2)->getPayRate(), kTolerance);
    CHECK_CLOSE(0.8, SickLeave::forDays(7, 3)->getPayRate(), kTolerance);
    CHECK_CLOSE(0.8, SickLeave::forDays(7, 4)->getPayRate(), kTolerance);
    CHECK_CLOSE(1.0, SickLeave::forDays(7, 5)->getPayRate(), kTolerance);
    CHECK_CLOSE(1.0, SickLeave::forDays(7, 25)->getPayRate(), kTolerance);
}

TEST(SickLeaveTest_ReportsSeniorityAndRejectsInvalidInput) {
    const std::unique_ptr<SickLeave> sick = SickLeave::forDays(3, 4);
    CHECK(sick != nullptr);
    CHECK_EQUAL(4, sick->getSeniorityYears());
    CHECK(sick->getTypeName().find("Отпуск") == std::string::npos);
    CHECK_EQUAL(0u, sick->getTypeName().find("Больничный"));

    CHECK_THROW(SickLeave(0, 5), InvalidInputException);
}

TEST(SickLeaveTest_NegativeSeniorityFallsBackToZero) {
    const SickLeave sick(1, -2);
    CHECK_EQUAL(0, sick.getSeniorityYears());
    CHECK_CLOSE(0.6, sick.getPayRate(), kTolerance);
}

TEST(WorkerAbsenceTest_AccumulatesDaysAndAveragesRates) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());

    CHECK_EQUAL(0, employee.getTotalAbsentDays());
    CHECK_CLOSE(1.0, employee.getAveragePayRate(), kTolerance);

    employee.addAbsence(SickLeave::forDays(7, 2));
    employee.addAbsence(Vacation::paid(14));
    employee.addAbsence(Absence::dayOff(1));

    CHECK_EQUAL(3u, employee.getAbsences().size());
    CHECK_EQUAL(22, employee.getTotalAbsentDays());
    CHECK_CLOSE(0.7, employee.getAveragePayRate(), kTolerance);

    CHECK_THROW(employee.addAbsence(nullptr), InvalidInputException);
}

TEST(WorkerOvertimeTest_AccumulatesUpToNorm) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());
    CHECK_CLOSE(0.0, employee.getOvertimeHours(), kTolerance);

    employee.registerOvertime(40.0);
    employee.registerOvertime(40.0);
    CHECK_CLOSE(Worker::kOvertimeLimitHours, employee.getOvertimeHours(), kTolerance);

    CHECK_THROW(employee.registerOvertime(0.5), OvertimeLimitExceededException);
    CHECK_THROW(employee.registerOvertime(0.0), InvalidInputException);
    CHECK_THROW(employee.registerOvertime(-10.0), InvalidInputException);
}

TEST(WorkerOvertimeTest_RejectedOvertimeDoesNotChangeState) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());
    employee.registerOvertime(20.0);

    CHECK_THROW(employee.registerOvertime(100.0), OvertimeLimitExceededException);
    CHECK_CLOSE(20.0, employee.getOvertimeHours(), kTolerance);
}

TEST(CompanySimulationTest_RegistersAbsencesForWorker) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    company.registerAbsence("IT-отдел", id, SickLeave::forDays(7, 4));
    company.registerAbsence("IT-отдел", id, Vacation::paid(14));

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    CHECK(developer != nullptr);
    CHECK_EQUAL(2u, developer->getAbsences().size());
    CHECK_EQUAL(21, developer->getTotalAbsentDays());
}

TEST(CompanySimulationTest_ReportsAddressingErrors) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    CHECK_THROW(company.registerAbsence("IT-отдел", 999, Absence::dayOff(1)),
                EmployeeNotFoundException);
    CHECK_THROW(company.registerAbsence("Юридический", id, Absence::dayOff(1)),
                DepartmentNotFoundException);
    CHECK_THROW(company.registerOvertime("Юридический", id, 1.0), DepartmentNotFoundException);

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    CHECK(developer != nullptr);
    CHECK_EQUAL(0, developer->getTotalAbsentDays());
}

TEST(CompanySimulationTest_EndPeriodClearsAccumulatedFacts) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    company.registerAbsence("IT-отдел", id, Vacation::unpaid(5));
    company.registerOvertime("IT-отдел", id, 12.0);
    company.addFunds(1000000.0);

    company.endPeriod();

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    CHECK(developer != nullptr);
    CHECK_EQUAL(0, developer->getTotalAbsentDays());
    CHECK(developer->getAbsences().empty());
    CHECK_CLOSE(0.0, developer->getOvertimeHours(), kTolerance);
    CHECK_CLOSE(1.0, developer->getAveragePayRate(), kTolerance);

    company.registerOvertime("IT-отдел", id, 60.0);
    company.registerOvertime("IT-отдел", id, 20.0);
    CHECK_CLOSE(Worker::kOvertimeLimitHours, developer->getOvertimeHours(), kTolerance);
}
