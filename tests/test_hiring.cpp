#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "company.hpp"
#include "core/worker_factory.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/hr_department.hpp"
#include "departments/it/workers/software_developer.hpp"
#include "departments/it/workers/system_administrator.hpp"
#include "departments/it_department.hpp"
#include "departments/production/workers/technician.hpp"
#include "departments/production_department.hpp"
#include "exceptions/department_not_found_exception.hpp"
#include "exceptions/duplicate_employee_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "workers/worker.hpp"

#include "test_helpers.hpp"

namespace {

// Company владеет отделами через unique_ptr, поэтому копировать и
// возвращавать её по значению нельзя — готовим компанию на месте.
void populateReadyCompany(Company& company) {
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.addDepartment(std::make_unique<ProductionDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);
}

constexpr double kDeveloperRate = 160000.0;

}  // namespace

TEST(RecruiterTest, ContractNumberFollowsSingleFormat) {
    EXPECT_EQ(Recruiter::contractNumberFor(7), "T-7");
    EXPECT_EQ(Recruiter::contractNumberFor(1), "T-1");
    EXPECT_EQ(Recruiter::contractNumberFor(1234), "T-1234");
}

TEST(RecruiterTest, ValidatesTermsAgainstAdvanceLimit) {
    Recruiter recruiter(1, "Смирнова О.О.", makeContract(), makeAdvance());

    EXPECT_NO_THROW(recruiter.validateTerms(100000.0, 15000.0));  // ровно 15%
    EXPECT_NO_THROW(recruiter.validateTerms(100000.0, 5000.0));
    EXPECT_THROW(recruiter.validateTerms(100000.0, 15001.0), InvalidInputException);
    EXPECT_THROW(recruiter.validateTerms(100000.0, 0.0), InvalidInputException);
    EXPECT_THROW(recruiter.validateTerms(0.0, 1000.0), InvalidInputException);
}

TEST(RecruiterTest, TracksVacancyPipeline) {
    Recruiter recruiter(1, "Смирнова О.О.", makeContract(), makeAdvance());
    EXPECT_EQ(recruiter.getVacanciesClosed(), 0);
    EXPECT_EQ(recruiter.getCandidatesInProcess(), 0);
    EXPECT_DOUBLE_EQ(recruiter.getClosingRate(), 0.0);

    recruiter.registerCandidate();
    recruiter.registerCandidate();
    EXPECT_EQ(recruiter.getCandidatesInProcess(), 2);
    EXPECT_DOUBLE_EQ(recruiter.getClosingRate(), 0.0);

    recruiter.closeVacancy();
    EXPECT_EQ(recruiter.getVacanciesClosed(), 1);
    EXPECT_EQ(recruiter.getCandidatesInProcess(), 1);  // один кандидат закрыт
    EXPECT_DOUBLE_EQ(recruiter.getClosingRate(), 0.5);
}

TEST(WorkerFactoryTest, CreatesEveryRoleWithCorrectPosition) {
    const std::vector<WorkerType> types{WorkerType::Accountant,
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

    std::vector<std::string> roles;
    for (WorkerType type : types) {
        std::unique_ptr<Worker> worker =
            WorkerFactory::create(1, "Тестовый", type, makeContract(), makeAdvance());
        ASSERT_NE(worker, nullptr) << "фабрика не создала " << WorkerFactory::positionFor(type);
        EXPECT_FALSE(worker->getRole().empty());
        EXPECT_FALSE(worker->getPosition().empty());
        roles.push_back(worker->getRole());
    }

    EXPECT_EQ(roles.size(), 12u);
    std::sort(roles.begin(), roles.end());
    EXPECT_EQ(std::adjacent_find(roles.begin(), roles.end()), roles.end());
}

TEST(WorkerFactoryTest, RejectsMissingTermsOfEmployment) {
    EXPECT_THROW(
        WorkerFactory::create(1, "Без контракта", WorkerType::Accountant, nullptr, makeAdvance()),
        InvalidInputException);
    EXPECT_THROW(
        WorkerFactory::create(1, "Без аванса", WorkerType::Accountant, makeContract(), nullptr),
        InvalidInputException);
}

TEST(CompanyTest, RejectsEmptyNameAndNullDepartment) {
    EXPECT_THROW(Company(""), InvalidInputException);
    EXPECT_THROW(Company("Acme").addDepartment(nullptr), InvalidInputException);
}

TEST(CompanyTest, AppointRecruiterBootstrapsHiring) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    // Найм невозможен без рекрутера, поэтому компания заводит первого сама.
    const Worker* recruiter = company.findDepartment("Отдел кадров")->findWorker(1);
    ASSERT_NE(recruiter, nullptr);
    EXPECT_EQ(recruiter->getRole(), "Recruiter");

    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);
    ASSERT_NE(developer, nullptr);
    EXPECT_EQ(developer->getPosition(), "Разработчик ПО");
}

TEST(CompanyTest, HireAssignsSequentialIdsAndContractNumber) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    Worker* first = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                       kDeveloperRate);
    Worker* second =
        company.hireWorker("IT-отдел", "Сидоров С.С.", WorkerType::SystemAdministrator, 140000.0);

    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first->getId(), second->getId());
    EXPECT_EQ(first->getContract().getContractNumber(),
              Recruiter::contractNumberFor(first->getId()));
    EXPECT_EQ(company.getTotalWorkersCount(), 3);  // рекрутер + двое
}

TEST(CompanyTest, HiredWorkerBelongsToDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);

    ASSERT_NE(developer, nullptr);
    Department* it = company.findDepartment("IT-отдел");
    ASSERT_NE(it, nullptr);
    EXPECT_TRUE(developer->worksIn(it));
    EXPECT_TRUE(it->hasWorker(developer->getId()));
}

TEST(CompanyTest, HireRejectsUnknownDepartmentAndDuplicateName) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    EXPECT_THROW(company.hireWorker("Юридический", "Петров П.П.", WorkerType::Lawyer, 130000.0),
                 DepartmentNotFoundException);
    EXPECT_THROW(company.hireWorker("IT-отдел", "", WorkerType::SoftwareDeveloper, kDeveloperRate),
                 InvalidInputException);
    EXPECT_THROW(company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, 0.0),
                 InvalidInputException);
}

TEST(CompanyTest, TerminateWorkerRemovesFromDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);
    ASSERT_NE(developer, nullptr);
    const int id = developer->getId();

    std::unique_ptr<Worker> terminated = company.terminateWorker("IT-отдел", id);
    ASSERT_NE(terminated, nullptr);
    EXPECT_FALSE(company.findDepartment("IT-отдел")->hasWorker(id));
    EXPECT_FALSE(terminated->getContract().isActiveOn(today()));

    EXPECT_THROW(company.terminateWorker("IT-отдел", id), EmployeeNotFoundException);
    EXPECT_THROW(company.terminateWorker("Нет такого", id), DepartmentNotFoundException);
}

TEST(CompanyTest, PayrollSumsUpEveryDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, kDeveloperRate);
    company.hireWorker("Производство", "Сидоров С.С.", WorkerType::Technician, 85000.0);

    const double payroll = company.calculateCompanyPayroll();
    const double itPayroll = company.findDepartment("IT-отдел")->calculatePayroll();

    EXPECT_GT(payroll, 0.0);
    EXPECT_GT(itPayroll, 0.0);
    EXPECT_LE(itPayroll, payroll);
    const std::string report = company.generateReport();
    EXPECT_NE(report.find("Acme LLC"), std::string::npos);
    EXPECT_NE(report.find("ФОТ"), std::string::npos);
    EXPECT_NE(report.find("сотрудников: 3"), std::string::npos);
}