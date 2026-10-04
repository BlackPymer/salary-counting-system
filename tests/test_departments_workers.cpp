#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "departments/accounting/workers/accountant.hpp"
#include "departments/accounting/workers/payroll_accountant.hpp"
#include "departments/accounting_department.hpp"
#include "departments/administration/workers/administrator.hpp"
#include "departments/administration_department.hpp"
#include "departments/customer_support/workers/customer_support_agent.hpp"
#include "departments/customer_support_department.hpp"
#include "departments/department.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/hr_department.hpp"
#include "departments/it/workers/software_developer.hpp"
#include "departments/it/workers/system_administrator.hpp"
#include "departments/it_department.hpp"
#include "departments/legal/workers/lawyer.hpp"
#include "departments/legal_department.hpp"
#include "departments/marketing/workers/marketer.hpp"
#include "departments/marketing_department.hpp"
#include "departments/production/workers/technician.hpp"
#include "departments/production_department.hpp"
#include "departments/research_n_developement/workers/research_scientist.hpp"
#include "departments/research_n_developement_department.hpp"
#include "departments/security/workers/security_guard.hpp"
#include "departments/security_department.hpp"
#include "exceptions/duplicate_employee_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "workers/manager.hpp"
#include "workers/specialist.hpp"

#include "test_helpers.hpp"

namespace {

constexpr double kTolerance = 0.001;
constexpr double kBaseRate = 100000.0;

// Строит компанию из десяти отделов — по одному на каждый тип отделов.
// Список собирается через push_back: initializer_list хранит элементы как
// const, и unique_ptr туда не переносится.
std::vector<std::unique_ptr<Department>> buildAllDepartments() {
    std::vector<std::unique_ptr<Department>> departments;
    departments.push_back(std::make_unique<AccountingDepartment>());
    departments.push_back(std::make_unique<AdministrationDepartment>());
    departments.push_back(std::make_unique<CustomerSupportDepartment>());
    departments.push_back(std::make_unique<HrDepartment>());
    departments.push_back(std::make_unique<ItDepartment>());
    departments.push_back(std::make_unique<LegalDepartment>());
    departments.push_back(std::make_unique<MarketingDepartment>());
    departments.push_back(std::make_unique<ProductionDepartment>());
    departments.push_back(std::make_unique<ResearchNDevelopementDepartment>());
    departments.push_back(std::make_unique<SecurityDepartment>());
    return departments;
}

// Одного сотрудника каждого типа, по одному отделу.
void hireOneOfEachRole(std::vector<std::unique_ptr<Department>>& departments) {
    int id = 1;
    departments[0]->addWorker(std::make_unique<Accountant>(
        id++, "Бухгалтер", makeContract(kBaseRate), makeAdvance(), "senior"));
    departments[0]->addWorker(std::make_unique<PayrollAccountant>(
        id++, "Бухгалтер по ЗП", makeContract(kBaseRate * 1.2), makeAdvance(), "chief", 30));
    departments[1]->addWorker(std::make_unique<Administrator>(
        id++, "Администратор", makeContract(kBaseRate), makeAdvance(), "B-1"));
    departments[2]->addWorker(std::make_unique<CustomerSupportAgent>(
        id++, "Оператор", makeContract(kBaseRate), makeAdvance(), "chat"));
    departments[3]->addWorker(
        std::make_unique<Recruiter>(id++, "Рекрутер", makeContract(kBaseRate), makeAdvance()));
    departments[4]->addWorker(std::make_unique<SoftwareDeveloper>(
        id++, "Разработчик", makeContract(kBaseRate * 1.6), makeAdvance(), "C++"));
    departments[4]->addWorker(std::make_unique<SystemAdministrator>(
        id++, "Сисадмин", makeContract(kBaseRate * 1.4), makeAdvance(), 8));
    departments[5]->addWorker(std::make_unique<Lawyer>(id++, "Юрист", makeContract(kBaseRate * 1.3),
                                                       makeAdvance(), "77-123"));
    departments[6]->addWorker(std::make_unique<Marketer>(
        id++, "Маркетолог", makeContract(kBaseRate * 1.1), makeAdvance()));
    departments[7]->addWorker(
        std::make_unique<Technician>(id++, "Техник", makeContract(kBaseRate * 0.85), makeAdvance(),
                                     std::vector<std::string>{"CNC", "Lathe"}));
    departments[8]->addWorker(std::make_unique<ResearchScientist>(
        id++, "Учёный", makeContract(kBaseRate * 1.5), makeAdvance(), "materials"));
    departments[9]->addWorker(std::make_unique<SecurityGuard>(
        id++, "Охранник", makeContract(kBaseRate * 0.65), makeAdvance(), "night"));
}

double roleBonusOf(const Worker* worker) {
    return worker->calculateSalary().getGross() - worker->getEffectiveRate();
}

}  // namespace

TEST(DepartmentTest, RejectsEmptyName) {
    EXPECT_THROW(Department(""), InvalidInputException);
}

TEST(DepartmentTest, RejectsNullWorker) {
    AccountingDepartment department;
    EXPECT_THROW(department.addWorker(nullptr), InvalidInputException);
}

TEST(DepartmentTest, RejectsDuplicateId) {
    AccountingDepartment department;
    department.addWorker(
        std::make_unique<Accountant>(1, "Первый", makeContract(), makeAdvance(), "junior"));

    EXPECT_THROW(department.addWorker(std::make_unique<Accountant>(1, "Второй", makeContract(),
                                                                   makeAdvance(), "junior")),
                 DuplicateEmployeeException);
    EXPECT_EQ(department.getWorkersCount(), 1u);
}

TEST(DepartmentTest, SetsAndClearsBackReference) {
    AccountingDepartment department;
    department.addWorker(
        std::make_unique<Accountant>(1, "Первый", makeContract(), makeAdvance(), "junior"));

    Worker* worker = department.findWorker(1);
    ASSERT_NE(worker, nullptr);
    EXPECT_TRUE(worker->worksIn(&department));

    std::unique_ptr<Worker> removed = department.removeWorker(1);
    EXPECT_EQ(removed->getDepartment(), nullptr);
    EXPECT_EQ(department.findWorker(1), nullptr);
}

TEST(DepartmentTest, RemoveUnknownWorkerThrows) {
    AccountingDepartment department;
    EXPECT_THROW(department.removeWorker(42), EmployeeNotFoundException);
    EXPECT_FALSE(department.hasWorker(42));
}

TEST(AllDepartmentsTest, ThereAreTenWithDistinctDescriptions) {
    const std::vector<std::unique_ptr<Department>> departments = buildAllDepartments();
    ASSERT_EQ(departments.size(), 10u);

    std::vector<std::string> names;
    for (const std::unique_ptr<Department>& department : departments) {
        EXPECT_FALSE(department->getDescription().empty());
        names.push_back(department->getName());
    }

    std::sort(names.begin(), names.end());
    EXPECT_EQ(std::adjacent_find(names.begin(), names.end()), names.end())
        << "названия отделов повторяются";
}

TEST(AllRolesTest, TwelveRolesHaveDistinctNamesAndSaneBonuses) {
    std::vector<std::unique_ptr<Department>> departments = buildAllDepartments();
    hireOneOfEachRole(departments);

    std::vector<std::string> roles;
    int totalWorkers = 0;
    for (const std::unique_ptr<Department>& department : departments) {
        for (const Worker* worker : department->getWorkers()) {
            roles.push_back(worker->getRole());

            // Надбавка всегда положительна и никогда не превышает ставку:
            // сотрудник не может заработать больше двух базовых ставок.
            const double bonus = roleBonusOf(worker);
            EXPECT_GT(bonus, 0.0) << worker->getRole() << ": надбавка нулевая";
            EXPECT_LT(bonus, worker->getBaseRate())
                << worker->getRole() << ": надбавка выше ставки";
            ++totalWorkers;
        }
    }

    EXPECT_EQ(totalWorkers, 12);

    std::sort(roles.begin(), roles.end());
    EXPECT_EQ(std::adjacent_find(roles.begin(), roles.end()), roles.end())
        << "названия ролей повторяются, роли не различаются";
}

TEST(AllRolesTest, InheritanceChainIsFourLevelsDeep) {
    AccountingDepartment department;
    department.addWorker(std::make_unique<PayrollAccountant>(1, "Бухгалтер по ЗП", makeContract(),
                                                             makeAdvance(), "junior", 5));

    Worker* worker = department.findWorker(1);
    ASSERT_NE(worker, nullptr);

    EXPECT_NE(dynamic_cast<PayrollAccountant*>(worker), nullptr);
    EXPECT_NE(dynamic_cast<Accountant*>(worker), nullptr);
    EXPECT_NE(dynamic_cast<Specialist*>(worker), nullptr);
    EXPECT_NE(dynamic_cast<Worker*>(worker), nullptr);
    EXPECT_EQ(dynamic_cast<Manager*>(worker), nullptr);
}

TEST(AccountantTest, SeniorCertificationRaisesBonus) {
    AccountingDepartment department;
    department.addWorker(
        std::make_unique<Accountant>(1, "Junior", makeContract(), makeAdvance(), "junior"));
    department.addWorker(
        std::make_unique<Accountant>(2, "Senior", makeContract(), makeAdvance(), "senior"));

    const double junior = roleBonusOf(department.findWorker(1));
    const double senior = roleBonusOf(department.findWorker(2));

    EXPECT_NEAR(junior, kBaseRate * 0.10, kTolerance);
    EXPECT_NEAR(senior, kBaseRate * 0.25, kTolerance);
    EXPECT_GT(senior, junior);
}

TEST(SoftwareDeveloperTest, FullCycleRaisesBonus) {
    ItDepartment department;
    department.addWorker(
        std::make_unique<SoftwareDeveloper>(1, "Dev", makeContract(), makeAdvance(), "C++"));

    SoftwareDeveloper* developer = dynamic_cast<SoftwareDeveloper*>(department.findWorker(1));
    ASSERT_NE(developer, nullptr);

    const double before = roleBonusOf(developer);
    EXPECT_FALSE(developer->isFullCycle());

    developer->completeTask();
    developer->reviewCode();
    EXPECT_TRUE(developer->isFullCycle());
    EXPECT_GT(roleBonusOf(developer), before);
}

TEST(SystemAdministratorTest, CriticalDowntimeRemovesBonus) {
    ItDepartment department;
    department.addWorker(
        std::make_unique<SystemAdministrator>(1, "Sys", makeContract(), makeAdvance(), 4));

    SystemAdministrator* admin = dynamic_cast<SystemAdministrator*>(department.findWorker(1));
    ASSERT_NE(admin, nullptr);

    EXPECT_NEAR(roleBonusOf(admin), kBaseRate * 0.20, kTolerance);

    admin->recordUptime(95.0);
    EXPECT_TRUE(admin->hasCriticalDowntime());
    EXPECT_NEAR(roleBonusOf(admin), kBaseRate * 0.11, kTolerance);

    EXPECT_THROW(admin->recordUptime(101.0), InvalidInputException);
    EXPECT_THROW(admin->recordUptime(-1.0), InvalidInputException);
}

TEST(SecurityGuardTest, NightShiftAddsBonus) {
    SecurityDepartment department;
    department.addWorker(
        std::make_unique<SecurityGuard>(1, "Дневной", makeContract(), makeAdvance(), "day"));
    department.addWorker(
        std::make_unique<SecurityGuard>(2, "Ночной", makeContract(), makeAdvance(), "night"));

    const double day = roleBonusOf(department.findWorker(1));
    const double night = roleBonusOf(department.findWorker(2));

    EXPECT_NEAR(day, kBaseRate * 0.08, kTolerance);
    EXPECT_NEAR(night, kBaseRate * 0.18, kTolerance);
}

TEST(ManagerTest, ApprovesLeaveOnlyForOwnDepartment) {
    AccountingDepartment department;
    department.addWorker(
        std::make_unique<Administrator>(1, "Главный", makeContract(), makeAdvance(), "B-1"));
    department.addWorker(
        std::make_unique<Accountant>(2, "Подчинённый", makeContract(), makeAdvance(), "junior"));

    Manager* chief = dynamic_cast<Manager*>(department.findWorker(1));
    ASSERT_NE(chief, nullptr);

    EXPECT_TRUE(chief->approveLeave(2));
    EXPECT_FALSE(chief->approveLeave(1)) << "руководитель не утверждает отпуск себе";
    EXPECT_FALSE(chief->approveLeave(999));
    const std::string report = chief->generateReport();
    EXPECT_NE(report.find("Главный"), std::string::npos);
    EXPECT_NE(report.find("в подчинении 2 чел."), std::string::npos);
}
