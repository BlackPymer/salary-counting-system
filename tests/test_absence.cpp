#include <gtest/gtest.h>

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

// Company владеет отделами через unique_ptr, поэтому готовим её на месте,
// а не возвращаем по значению.
void populateCompanyWithDeveloper(Company& company, int& developerId) {
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);

    Worker* developer =
        company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, 160000.0);
    developerId = developer->getId();
}

}  // namespace

TEST(AbsenceTest, RejectsInvalidDays) {
    EXPECT_THROW(Absence(0, Absence::kFullPay), InvalidInputException);
    EXPECT_THROW(Absence(-3, Absence::kFullPay), InvalidInputException);
    EXPECT_THROW(Absence::dayOff(0), InvalidInputException);
}

TEST(AbsenceTest, DayOffIsPaidPartially) {
    const std::unique_ptr<Absence> dayOff = Absence::dayOff(2);

    ASSERT_NE(dayOff, nullptr);
    EXPECT_EQ(dayOff->getDays(), 2);
    EXPECT_DOUBLE_EQ(dayOff->getPayRate(), Absence::kHalfPay);
}

TEST(VacationTest, PaidAndUnpaidDifferOnlyByRate) {
    const std::unique_ptr<Vacation> paid = Vacation::paid(14);
    const std::unique_ptr<Vacation> unpaid = Vacation::unpaid(14);

    ASSERT_NE(paid, nullptr);
    ASSERT_NE(unpaid, nullptr);
    EXPECT_EQ(paid->getDays(), unpaid->getDays());
    EXPECT_DOUBLE_EQ(paid->getPayRate(), Absence::kFullPay);
    EXPECT_DOUBLE_EQ(unpaid->getPayRate(), Absence::kNoPay);
    EXPECT_NE(paid->getTypeName(), unpaid->getTypeName());
}

TEST(VacationTest, IsUsableThroughBaseReference) {
    const Vacation paid(5, true);
    const Vacation unpaid(5, false);
    const Absence& asPaid = paid;
    const Absence& asUnpaid = unpaid;

    EXPECT_DOUBLE_EQ(asPaid.getPayRate(), 1.0);
    EXPECT_DOUBLE_EQ(asUnpaid.getPayRate(), 0.0);
}

TEST(SickLeaveTest, PayRateDependsOnSeniority) {
    EXPECT_NEAR(SickLeave::forDays(7, 0)->getPayRate(), 0.6, kTolerance);
    EXPECT_NEAR(SickLeave::forDays(7, 2)->getPayRate(), 0.6, kTolerance);
    EXPECT_NEAR(SickLeave::forDays(7, 3)->getPayRate(), 0.8, kTolerance);
    EXPECT_NEAR(SickLeave::forDays(7, 4)->getPayRate(), 0.8, kTolerance);
    EXPECT_NEAR(SickLeave::forDays(7, 5)->getPayRate(), 1.0, kTolerance);
    EXPECT_NEAR(SickLeave::forDays(7, 25)->getPayRate(), 1.0, kTolerance);
}

TEST(SickLeaveTest, ReportsSeniorityAndRejectsInvalidInput) {
    const std::unique_ptr<SickLeave> sick = SickLeave::forDays(3, 4);
    ASSERT_NE(sick, nullptr);
    EXPECT_EQ(sick->getSeniorityYears(), 4);
    EXPECT_EQ(sick->getTypeName().find("Отпуск"), std::string::npos)
        << "больничный не должен выглядеть как отпуск";
    EXPECT_EQ(sick->getTypeName().find("Больничный"), 0u);

    EXPECT_THROW(SickLeave(0, 5), InvalidInputException);
}

TEST(SickLeaveTest, NegativeSeniorityFallsBackToZero) {
    // Стаж не может быть отрицательным, но вместо исключения приводится к нулю:
    // выплата становится минимальной, а объект остаётся валидным.
    const SickLeave sick(1, -2);
    EXPECT_EQ(sick.getSeniorityYears(), 0);
    EXPECT_NEAR(sick.getPayRate(), 0.6, kTolerance);
}

TEST(WorkerAbsenceTest, AccumulatesDaysAndAveragesRates) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());

    EXPECT_EQ(employee.getTotalAbsentDays(), 0);
    EXPECT_DOUBLE_EQ(employee.getAveragePayRate(), 1.0)
        << "без отсутствий ставка должна быть полной";

    employee.addAbsence(SickLeave::forDays(7, 2));  // 0.6
    employee.addAbsence(Vacation::paid(14));        // 1.0
    employee.addAbsence(Absence::dayOff(1));        // 0.5

    EXPECT_EQ(employee.getAbsences().size(), 3u);
    EXPECT_EQ(employee.getTotalAbsentDays(), 22);
    EXPECT_NEAR(employee.getAveragePayRate(), 0.7, kTolerance);

    EXPECT_THROW(employee.addAbsence(nullptr), InvalidInputException);
}

TEST(WorkerOvertimeTest, AccumulatesUpToNorm) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());
    EXPECT_DOUBLE_EQ(employee.getOvertimeHours(), 0.0);

    employee.registerOvertime(40.0);
    employee.registerOvertime(40.0);
    EXPECT_DOUBLE_EQ(employee.getOvertimeHours(), Worker::kOvertimeLimitHours);

    EXPECT_THROW(employee.registerOvertime(0.5), OvertimeLimitExceededException);
    EXPECT_THROW(employee.registerOvertime(0.0), InvalidInputException);
    EXPECT_THROW(employee.registerOvertime(-10.0), InvalidInputException);
}

TEST(WorkerOvertimeTest, RejectedOvertimeDoesNotChangeState) {
    Worker employee(1, "Тестовый", makeContract(), makeAdvance());
    employee.registerOvertime(20.0);

    EXPECT_THROW(employee.registerOvertime(100.0), OvertimeLimitExceededException);
    EXPECT_DOUBLE_EQ(employee.getOvertimeHours(), 20.0)
        << "отклонённая попытка не должна сдвигать накопленные часы";
}

TEST(CompanySimulationTest, RegistersAbsencesForWorker) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    company.registerAbsence("IT-отдел", id, SickLeave::forDays(7, 4));
    company.registerAbsence("IT-отдел", id, Vacation::paid(14));

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    ASSERT_NE(developer, nullptr);
    EXPECT_EQ(developer->getAbsences().size(), 2u);
    EXPECT_EQ(developer->getTotalAbsentDays(), 21);
}

TEST(CompanySimulationTest, ReportsAddressingErrors) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    EXPECT_THROW(company.registerAbsence("IT-отдел", 999, Absence::dayOff(1)),
                 EmployeeNotFoundException);
    EXPECT_THROW(company.registerAbsence("Юридический", id, Absence::dayOff(1)),
                 DepartmentNotFoundException);
    EXPECT_THROW(company.registerOvertime("Юридический", id, 1.0), DepartmentNotFoundException);

    // Отсутствие несуществующего сотрудника не должно оставлять следов.
    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    ASSERT_NE(developer, nullptr);
    EXPECT_EQ(developer->getTotalAbsentDays(), 0);
}

TEST(CompanySimulationTest, EndPeriodClearsAccumulatedFacts) {
    int id = 0;
    Company company{"Acme LLC"};
    populateCompanyWithDeveloper(company, id);

    company.registerAbsence("IT-отдел", id, Vacation::unpaid(5));
    company.registerOvertime("IT-отдел", id, 12.0);

    company.endPeriod();

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(id);
    ASSERT_NE(developer, nullptr);
    EXPECT_EQ(developer->getTotalAbsentDays(), 0);
    EXPECT_TRUE(developer->getAbsences().empty());
    EXPECT_DOUBLE_EQ(developer->getOvertimeHours(), 0.0);
    EXPECT_DOUBLE_EQ(developer->getAveragePayRate(), 1.0);

    // После закрытия периода накопление начинается заново: 80 ч снова
    // набираются с нуля, а не продолжаются с прошлого месяца.
    company.registerOvertime("IT-отдел", id, 60.0);
    EXPECT_NO_THROW(company.registerOvertime("IT-отдел", id, 20.0))
        << "счётчик часов должен обнулиться, а не продолжаться";
    EXPECT_DOUBLE_EQ(developer->getOvertimeHours(), Worker::kOvertimeLimitHours);
}