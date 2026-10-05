#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <string>
#include <vector>

#include "attendance/absence.hpp"
#include "company.hpp"
#include "core/worker_factory.hpp"
#include "departments/accounting_department.hpp"
#include "departments/department.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/hr_department.hpp"
#include "departments/it_department.hpp"
#include "departments/legal/workers/lawyer.hpp"
#include "exceptions/contract_expired_exception.hpp"
#include "exceptions/department_not_found_exception.hpp"
#include "exceptions/duplicate_employee_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/file_io_exception.hpp"
#include "exceptions/insufficient_funds_exception.hpp"
#include "exceptions/invalid_contract_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "exceptions/overtime_limit_exceeded_exception.hpp"
#include "exceptions/payment_failed_exception.hpp"
#include "exceptions/salary_calculation_exception.hpp"
#include "exceptions/tax_calculation_exception.hpp"
#include "exceptions/unauthorized_access_exception.hpp"
#include "financial_objects/payroll_period.hpp"
#include "financial_objects/probation_period.hpp"
#include "financial_objects/salary.hpp"
#include "payroll/calculation_context.hpp"
#include "payroll/strategies/tax_strategy.hpp"
#include "workers/manager.hpp"
#include "workers/specialist.hpp"
#include "workers/worker.hpp"

#include "test_helpers.hpp"

namespace {
constexpr double kTolerance = 0.001;
constexpr double kRate = 100000.0;

std::vector<WorkerType> allWorkerTypes() {
    return {WorkerType::Accountant,
            WorkerType::PayrollAccountant,
            WorkerType::Administrator,
            WorkerType::CustomerSupportAgent,
            WorkerType::Recruiter,
            WorkerType::SoftwareDeveloper,
            WorkerType::SystemAdministrator,
            WorkerType::Lawyer,
            WorkerType::Marketer,
            WorkerType::Technician,
            WorkerType::ResearchScientist,
            WorkerType::SecurityGuard};
}
}  // namespace

TEST(WorkerFactoryPositionFor_CoversEveryType) {
    for (const WorkerType type : allWorkerTypes()) {
        CHECK(!WorkerFactory::positionFor(type).empty());
    }
}

TEST(WorkerFactoryPositionFor_RejectsUnknownType) {
    CHECK_THROW(WorkerFactory::positionFor(static_cast<WorkerType>(99)), InvalidInputException);
    CHECK_THROW(
        WorkerFactory::create(1, "X", static_cast<WorkerType>(99), makeContract(), makeAdvance()),
        InvalidInputException);
}

TEST(CalculationContextTest_NullWorkerThrowsInBothConstructors) {
    const PayrollPeriod period(2026, 10);
    CHECK_THROW(CalculationContext(nullptr, period), InvalidInputException);
    CHECK_THROW(CalculationContext(nullptr, period, period.getLastDay()), InvalidInputException);
}

TEST(CalculationContextTest_CustomCalculationDateIsKept) {
    auto worker = WorkerFactory::create(1, "Иван Иванов", WorkerType::SoftwareDeveloper,
                                        makeContract(kRate), makeAdvance());
    const PayrollPeriod period(2026, 10);
    const Date custom = period.getFirstDay();
    const CalculationContext ctx(worker.get(), period, custom);

    CHECK_EQUAL(custom, ctx.getCalculationDate());
    CHECK_EQUAL(10, ctx.getPeriod().getMonth());
}

TEST(CalculationContextTest_DelegatesOvertimeAbsencesAndContract) {
    auto worker = WorkerFactory::create(1, "Иван Иванов", WorkerType::SoftwareDeveloper,
                                        makeContract(kRate), makeAdvance());
    const PayrollPeriod period(2026, 10);
    CalculationContext ctx(worker.get(), period);

    worker->registerOvertime(5.0);
    worker->addAbsence(Absence::dayOff(2));

    CHECK_EQUAL(&worker->getContract(), &ctx.getContract());
    CHECK_EQUAL(worker->getId(), ctx.getWorker().getId());
    CHECK_CLOSE(5.0, ctx.getOvertimeHours(), kTolerance);
    CHECK_EQUAL(2, ctx.getTotalAbsentDays());
    CHECK_CLOSE(worker->getAveragePayRate(), ctx.getAveragePayRate(), kTolerance);
}
TEST(WorkerTax_SalaryCoversAllProgressiveBrackets) {
    struct Case {
        double rate;
        double expectedTax;
    };
    const std::vector<Case> cases{
        {300000.0, 200000.0 * 0.13 + 100000.0 * 0.15},
        {500000.0, 200000.0 * 0.13 + 200000.0 * 0.15 + 100000.0 * 0.18},
        {700000.0, 200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + 100000.0 * 0.20},
        {900000.0,
         200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + 200000.0 * 0.20 + 100000.0 * 0.22},
    };

    int id = 1;
    for (const Case& item : cases) {
        auto contract =
            std::make_unique<EmploymentContract>("T-TEST", "Должность", today(), item.rate, false);
        Worker worker(id++, "Налогоплательщик", std::move(contract), makeAdvance());
        const Salary salary = worker.calculateSalary();
        CHECK_CLOSE(item.rate, salary.getGross(), kTolerance);
        CHECK_CLOSE(item.expectedTax, salary.getTaxDeduction(), kTolerance);
    }
}

TEST(WorkerBasics_HireDateContractAndRole) {
    auto contract = makeContract(kRate);
    const Date hireDate = contract->getHireDate();
    Worker worker(1, "Прямой", std::move(contract), makeAdvance());

    CHECK_EQUAL("Worker", worker.getRole());
    CHECK_EQUAL(hireDate, worker.getHireDate());
    CHECK_CLOSE(kRate, worker.getContract().getMonthlyRate(), kTolerance);
    CHECK_EQUAL("Должность", worker.getPosition());
}

TEST(WorkerAdvance_RepayWithoutAdvanceReturnsZero) {
    Worker worker(2, "Без аванса", nullptr, nullptr);
    CHECK_CLOSE(0.0, worker.repayAdvance(5000.0), kTolerance);
    worker.resetAdvance();
}

TEST(TaxStrategyTest_AppliesProgressiveBrackets) {
    auto worker = WorkerFactory::create(1, "Иван", WorkerType::SoftwareDeveloper,
                                        makeContract(kRate), makeAdvance());
    const PayrollPeriod period(2026, 10);
    const CalculationContext ctx(worker.get(), period);
    const TaxStrategy strategy;

    Salary zero(0.0, 0.0, 0.0);
    strategy.apply(ctx, zero);
    CHECK_CLOSE(0.0, zero.getTaxDeduction(), kTolerance);

    Salary low(150000.0, 0.0, 150000.0);
    strategy.apply(ctx, low);
    CHECK_CLOSE(150000.0 * 0.13, low.getTaxDeduction(), kTolerance);

    Salary mid(500000.0, 0.0, 500000.0);
    strategy.apply(ctx, mid);
    CHECK_CLOSE(200000.0 * 0.13 + 200000.0 * 0.15 + 100000.0 * 0.18, mid.getTaxDeduction(),
                kTolerance);

    Salary high(700000.0, 0.0, 700000.0);
    strategy.apply(ctx, high);
    CHECK_CLOSE(200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + 100000.0 * 0.20,
                high.getTaxDeduction(), kTolerance);

    Salary top(1000000.0, 0.0, 1000000.0);
    strategy.apply(ctx, top);
    CHECK_CLOSE(
        200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + 200000.0 * 0.20 + 200000.0 * 0.22,
        top.getTaxDeduction(), kTolerance);
}

TEST(SalaryTest_ToStringAndRepaymentExceedingNetThrows) {
    Salary salary(1500.0, 195.0, 1305.0);
    const std::string text = salary.toString();
    CHECK(text.find("gross=1500.00") != std::string::npos);
    CHECK(text.find("tax=195.00") != std::string::npos);
    CHECK(text.find("net=1305.00") != std::string::npos);

    CHECK_THROW(salary.applyRepayment(1306.0), InvalidInputException);
}

TEST(AdvancePaymentTest_ExposesAmountRepaidAndIssueDate) {
    const Date issued = daysBefore(today(), 10);
    AdvancePayment advance(50000.0, issued);

    CHECK_CLOSE(50000.0, advance.getAmount(), kTolerance);
    CHECK_CLOSE(0.0, advance.getRepaidAmount(), kTolerance);
    CHECK_EQUAL(issued, advance.getIssueDate());

    advance.applyDeduction(20000.0);
    CHECK_CLOSE(20000.0, advance.getRepaidAmount(), kTolerance);
    CHECK_CLOSE(30000.0, advance.getRemaining(), kTolerance);
}

TEST(PayrollPeriodTest_DefaultCtorUsesCurrentDate) {
    const PayrollPeriod period;
    const Date now = today();
    CHECK_EQUAL(static_cast<int>(now.year()), period.getYear());
    CHECK_EQUAL(static_cast<int>(static_cast<unsigned int>(now.month())), period.getMonth());
    CHECK(period.contains(now));
}

TEST(PayrollPeriodTest_ContainsRejectsInvalidAndForeignDates) {
    const PayrollPeriod period(2026, 10);
    const Date invalid{};
    const Date otherYear{std::chrono::year{2020}, std::chrono::January, std::chrono::day{15}};
    const Date otherMonth{std::chrono::year{2026}, std::chrono::March, std::chrono::day{15}};

    CHECK(!period.contains(invalid));
    CHECK(!period.contains(otherYear));
    CHECK(!period.contains(otherMonth));
}

TEST(ProbationPeriodTest_CompletedAndRemainingBeforeStart) {
    const ProbationPeriod past(daysBefore(today(), 40), 30);
    CHECK(!past.isCompleted());

    const ProbationPeriod future(today(), 30);
    CHECK(future.isCompleted());
    CHECK_EQUAL(30, future.daysRemaining(daysBefore(today(), 5)));
    CHECK_EQUAL(0, future.daysRemaining(daysAfter(today(), 40)));
}

TEST(AbsenceTest_PayRateValidationAndBaseTypeName) {
    CHECK_THROW(Absence(3, 1.5), InvalidInputException);
    CHECK_THROW(Absence(3, -0.1), InvalidInputException);
    CHECK_THROW(Absence(0, 0.5), InvalidInputException);

    Absence absence(3, 0.5);
    CHECK_EQUAL("Отсутствие", absence.getTypeName());
    CHECK_EQUAL(3, absence.getDays());
    CHECK_CLOSE(0.5, absence.getPayRate(), kTolerance);
}

TEST(SpecialistTest_RoleSpecializationAndBonusRate) {
    Specialist specialist(1, "Специалист", makeContract(kRate), makeAdvance());

    CHECK_EQUAL("Specialist", specialist.getRole());
    CHECK_EQUAL("Должность", specialist.getSpecialization());
    CHECK_CLOSE(0.10, specialist.getSpecializationBonusRate(), kTolerance);

    const double gross = specialist.calculateSalary().getGross();
    CHECK(gross >= kRate * 0.85 + kRate * 0.10 - kTolerance);
}

TEST(ManagerTest_RoleReportAndBonusWithoutDepartment) {
    Manager manager(1, "Начальник", makeContract(kRate), makeAdvance());

    CHECK_EQUAL("Manager", manager.getRole());

    const std::string report = manager.generateReport();
    CHECK(report.find("Начальник") != std::string::npos);
    CHECK(report.find("Отчёт руководителя") != std::string::npos);

    CHECK(!manager.approveLeave(1));
    CHECK(!manager.approveLeave(99));

    const double gross = manager.calculateSalary().getGross();
    CHECK(gross >= kRate * 0.85 + kRate * 0.20 - kTolerance);
}

TEST(RecruiterTest_SpecializationName) {
    const Recruiter recruiter(1, "Рекрутер", makeContract(kRate), makeAdvance());
    CHECK_EQUAL("Подбор и адаптация персонала", recruiter.getSpecialization());
}

TEST(RecruiterTest_CreateAdvanceRejectsNonPositiveAmount) {
    const Recruiter recruiter(1, "Рекрутер", makeContract(kRate), makeAdvance());
    CHECK_THROW(recruiter.createAdvance(0.0), InvalidInputException);
    CHECK_THROW(recruiter.createAdvance(-100.0), InvalidInputException);

    const std::unique_ptr<AdvancePayment> advance = recruiter.createAdvance(15000.0);
    CHECK(advance != nullptr);
    CHECK_CLOSE(15000.0, advance->getAmount(), kTolerance);
}

TEST(LawyerTest_EmptyBarNumberRejectedAndSpecializationIncludesNumber) {
    CHECK_THROW(Lawyer(1, "Без номера", makeContract(kRate), makeAdvance(), ""),
                InvalidInputException);

    const Lawyer lawyer(2, "Юрист", makeContract(kRate), makeAdvance(), "77-555");
    const std::string spec = lawyer.getSpecialization();
    CHECK(spec.find("Юридическое сопровождение") != std::string::npos);
    CHECK(spec.find("77-555") != std::string::npos);
}

TEST(DepartmentTest_BaseDescriptionMentionsNameAndCount) {
    const Department department("Тестовый отдел");
    CHECK_EQUAL("Отдел 'Тестовый отдел', сотрудников: 0", department.getDescription());
}

TEST(CompanyTest_AddDuplicateDepartmentThrows) {
    Company company("Acme");
    company.addDepartment(std::make_unique<ItDepartment>());
    CHECK_THROW(company.addDepartment(std::make_unique<ItDepartment>()), InvalidInputException);
    CHECK_EQUAL(1u, company.getDepartmentsCount());
}

TEST(CompanyTest_AppointRecruiterRequiresHrDepartment) {
    Company company("Acme");
    company.addDepartment(std::make_unique<ItDepartment>());
    CHECK_THROW(company.appointRecruiter("", 90000.0), InvalidInputException);
    CHECK_THROW(company.appointRecruiter("Иванова М.С.", 90000.0), DepartmentNotFoundException);
    CHECK_EQUAL(1u, company.getDepartmentsCount());
}

TEST(CompanyTest_HireRejectsEmptyNameBeforeLookup) {
    Company company("Acme");
    company.addDepartment(std::make_unique<ItDepartment>());
    CHECK_THROW(company.hireWorker("IT-отдел", "", WorkerType::SoftwareDeveloper, kRate),
                InvalidInputException);
}

TEST(CompanyTest_HireWithoutRecruiterThrowsAndFindsNone) {
    Company company("Acme");
    company.addDepartment(std::make_unique<ItDepartment>());
    CHECK_THROW(company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, kRate),
                EmployeeNotFoundException);
}

TEST(CompanyTest_PayInactiveWorkerThrowsDuringEndPeriod) {
    Company company("Acme");
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);
    company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, kRate);
    company.addFunds(10000000.0);

    Worker* developer = company.findDepartment("IT-отдел")->findWorker(2);
    CHECK(developer != nullptr);
    developer->getContract().terminate(today());

    CHECK_THROW(company.endPeriod(), PaymentFailedException);
}

TEST(CompanyTest_RegisterAbsenceReportsMissingWorkerAndDepartment) {
    Company company("Acme");
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);
    company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, kRate);

    try {
        company.registerAbsence("IT-отдел", 999, Absence::dayOff(1));
        CHECK(false);
    } catch (const EmployeeNotFoundException& e) {
        CHECK_EQUAL(999, e.getEmployeeId());
        CHECK_EQUAL("IT-отдел", e.getDepartmentName());
    }

    try {
        company.registerOvertime("Юридический отдел", 1, 5.0);
        CHECK(false);
    } catch (const DepartmentNotFoundException& e) {
        CHECK_EQUAL("Юридический отдел", e.getDepartmentName());
    }

    company.registerOvertime("IT-отдел", 2, 5.0);
    CHECK_CLOSE(5.0, company.findDepartment("IT-отдел")->findWorker(2)->getOvertimeHours(),
                kTolerance);
}

TEST(CompanyTest_GenerateReportMentionsEveryDepartmentDescription) {
    Company company("Acme");
    company.addDepartment(std::make_unique<ItDepartment>());
    company.addDepartment(std::make_unique<AccountingDepartment>());

    const std::string report = company.generateReport();
    CHECK(report.find("Компания 'Acme'") != std::string::npos);
    CHECK(report.find(company.findDepartment("IT-отдел")->getDescription()) != std::string::npos);
    CHECK(report.find(company.findDepartment("Бухгалтерия")->getDescription()) !=
          std::string::npos);
    CHECK(report.find("ФОТ:") != std::string::npos);
    CHECK(report.find("сотрудников: 0") != std::string::npos);
}

TEST(DepartmentTest_RemoveExistingWorkerSucceeds) {
    ItDepartment department;
    department.addWorker(
        std::make_unique<Specialist>(7, "Уходящий", makeContract(kRate), makeAdvance()));

    CHECK(department.hasWorker(7));
    std::unique_ptr<Worker> removed = department.removeWorker(7);
    CHECK(removed != nullptr);
    CHECK_EQUAL(7, removed->getId());
    CHECK(!department.hasWorker(7));
    CHECK_EQUAL(0u, department.getWorkersCount());
    CHECK(removed->worksIn(nullptr));
    CHECK(department.getWorkers().empty());
}

TEST(ExceptionsTest_ExposedGettersAndMessageConstructors) {
    const DepartmentNotFoundException deptError("Юридический отдел");
    CHECK_EQUAL("Юридический отдел", deptError.getDepartmentName());
    CHECK(std::string(deptError.what()).find("Юридический отдел") != std::string::npos);

    const DuplicateEmployeeException dupError(42, "Петров П.П.");
    CHECK_EQUAL(42, dupError.getEmployeeId());
    CHECK_EQUAL("Петров П.П.", dupError.getFullName());
    CHECK(std::string(dupError.what()).find("42") != std::string::npos);

    const EmployeeNotFoundException empError(7, "IT-отдел");
    CHECK_EQUAL(7, empError.getEmployeeId());
    CHECK_EQUAL("IT-отдел", empError.getDepartmentName());
    CHECK(std::string(empError.what()).find("IT-отдел") != std::string::npos);

    const ContractExpiredException expiredString(std::string("истёк"));
    CHECK(std::string(expiredString.what()).find("истёк") != std::string::npos);
    const ContractExpiredException expiredLiteral("истёк");
    CHECK(std::string(expiredLiteral.what()).find("истёк") != std::string::npos);

    const FileIoException fileString(std::string("нет файла"));
    CHECK(std::string(fileString.what()).find("нет файла") != std::string::npos);
    const FileIoException fileLiteral("нет файла");
    CHECK(std::string(fileLiteral.what()).find("нет файла") != std::string::npos);

    const InvalidContractException contractString(std::string("bad"));
    CHECK(std::string(contractString.what()).find("bad") != std::string::npos);
    const InvalidContractException contractLiteral("bad");
    CHECK(std::string(contractLiteral.what()).find("bad") != std::string::npos);
}

TEST(ExceptionsTest_ErrorCodes) {
    CHECK_EQUAL(4001, ContractExpiredException("x").getErrorCode());
    CHECK_EQUAL(4002, DepartmentNotFoundException("Отдел").getErrorCode());
    CHECK_EQUAL(4003, DuplicateEmployeeException(1, "x").getErrorCode());
    CHECK_EQUAL(4004, EmployeeNotFoundException(1, "Отдел").getErrorCode());
    CHECK_EQUAL(4005, FileIoException("x").getErrorCode());
    CHECK_EQUAL(4006, InsufficientFundsException("x").getErrorCode());
    CHECK_EQUAL(4007, InvalidContractException("x").getErrorCode());
    CHECK_EQUAL(4008, InvalidInputException("x").getErrorCode());
    CHECK_EQUAL(4009, OvertimeLimitExceededException("x", 5.0, 2.0).getErrorCode());
    CHECK_EQUAL(4010, PaymentFailedException("x").getErrorCode());
    CHECK_EQUAL(4011, SalaryCalculationException("x").getErrorCode());
    CHECK_EQUAL(4012, TaxCalculationException("x").getErrorCode());
    CHECK_EQUAL(4013, UnauthorizedAccessException("x").getErrorCode());
}

TEST(DepartmentTest_HeadcountLimitAndBudget) {
    const Department defaults("Отдел по умолчанию");
    CHECK_EQUAL(Department::kDefaultHeadcountLimit, defaults.getHeadcountLimit());
    CHECK_CLOSE(Department::kDefaultMonthlyBudget, defaults.getMonthlyBudget(), kTolerance);

    const Department custom("Отдел", "Описание отдела", 5, 250000.0);
    CHECK_EQUAL(5, custom.getHeadcountLimit());
    CHECK_CLOSE(250000.0, custom.getMonthlyBudget(), kTolerance);
    CHECK_EQUAL("Описание отдела, сотрудников: 0", custom.getDescription());

    CHECK_THROW(Department("Отдел", "Описание", 0, 100.0), InvalidInputException);
    CHECK_THROW(Department("Отдел", "Описание", 10, 0.0), InvalidInputException);
}

TEST(DepartmentTest_HeadcountLimitRejectsOverflow) {
    Department tiny("Крошечный", "Описание", 1, 100000.0);
    tiny.addWorker(std::make_unique<Specialist>(1, "Первый", makeContract(), makeAdvance()));
    CHECK_THROW(
        tiny.addWorker(std::make_unique<Specialist>(2, "Второй", makeContract(), makeAdvance())),
        InvalidInputException);
    CHECK_EQUAL(1u, tiny.getWorkersCount());
}

TEST(PayrollPeriodTest_DaysInMonth) {
    CHECK_EQUAL(29, PayrollPeriod(2024, 2).getDaysInMonth());
    CHECK_EQUAL(30, PayrollPeriod(2023, 4).getDaysInMonth());
    CHECK_EQUAL(31, PayrollPeriod(2025, 1).getDaysInMonth());
    const PayrollPeriod current;
    CHECK(current.getDaysInMonth() >= 28);
    CHECK(current.getDaysInMonth() <= 31);
}

TEST(AdvancePaymentTest_RepaymentDueIsThirtyDaysAfterIssue) {
    const Date issued{std::chrono::year{2025}, std::chrono::January, std::chrono::day{15}};
    const AdvancePayment advance(1000.0, issued);
    const Date due = advance.getRepaymentDue();
    CHECK_EQUAL(2025, static_cast<int>(due.year()));
    CHECK_EQUAL(2, static_cast<unsigned>(due.month()));
    CHECK_EQUAL(14, static_cast<unsigned>(due.day()));
}

TEST(EmploymentContractTest_RenewalCount) {
    EmploymentContract contract("T-1", "Должность", today(), 100000.0);
    CHECK_EQUAL(0, contract.getRenewalCount());
    contract.renew(110000.0, daysAfter(today(), 30));
    contract.renew(120000.0, daysAfter(today(), 60));
    CHECK_EQUAL(2, contract.getRenewalCount());
}

TEST(SalaryTest_AccumulatesBonusesAndDeductions) {
    Salary salary;
    salary.applyBonus(5000.0);
    salary.applyBonus(1500.0);
    salary.applyDeduction(700.0);
    CHECK_CLOSE(6500.0, salary.getBonusesTotal(), kTolerance);
    CHECK_CLOSE(700.0, salary.getDeductionsTotal(), kTolerance);
}

TEST(CompanyTest_RequisitesInReport) {
    Company company("Симуля-ЛТД", "г. Москва, ул. Зарплатная, 1", "7700000000", 2010,
                    "Разработка ПО", "Иванов И.И.", "https://salary.example.com");
    CHECK_EQUAL("г. Москва, ул. Зарплатная, 1", company.getAddress());
    CHECK_EQUAL("7700000000", company.getTaxId());
    CHECK_EQUAL(2010, company.getFoundedYear());
    CHECK_EQUAL("Разработка ПО", company.getIndustry());
    CHECK_EQUAL("Иванов И.И.", company.getCeoName());
    CHECK_EQUAL("https://salary.example.com", company.getWebsite());

    const std::string report = company.generateReport();
    CHECK(report.find("Компания 'Симуля-ЛТД'") != std::string::npos);
    CHECK(report.find("ИНН 7700000000") != std::string::npos);
    CHECK(report.find("основан: 2010") != std::string::npos);
    CHECK(report.find("директор: Иванов И.И.") != std::string::npos);

    CHECK_THROW(Company("X", "", "", -1, "", "", ""), InvalidInputException);
}
