#include <UnitTest++/UnitTest++.h>

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

void populateReadyCompany(Company& company) {
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.addDepartment(std::make_unique<ProductionDepartment>());
    company.appointRecruiter("Смирнова О.О.", 95000.0);
}

constexpr double kDeveloperRate = 160000.0;

}  // namespace

TEST(RecruiterTest_ContractNumberFollowsSingleFormat) {
    CHECK_EQUAL("T-7", Recruiter::contractNumberFor(7));
    CHECK_EQUAL("T-1", Recruiter::contractNumberFor(1));
    CHECK_EQUAL("T-1234", Recruiter::contractNumberFor(1234));
}

TEST(RecruiterTest_ValidatesTermsAgainstAdvanceLimit) {
    Recruiter recruiter(1, "Смирнова О.О.", makeContract(), makeAdvance());

    recruiter.validateTerms(100000.0, 15000.0);
    recruiter.validateTerms(100000.0, 5000.0);
    CHECK_THROW(recruiter.validateTerms(100000.0, 15001.0), InvalidInputException);
    CHECK_THROW(recruiter.validateTerms(100000.0, 0.0), InvalidInputException);
    CHECK_THROW(recruiter.validateTerms(0.0, 1000.0), InvalidInputException);
}

TEST(RecruiterTest_TracksVacancyPipeline) {
    Recruiter recruiter(1, "Смирнова О.О.", makeContract(), makeAdvance());
    CHECK_EQUAL(0, recruiter.getVacanciesClosed());
    CHECK_EQUAL(0, recruiter.getCandidatesInProcess());
    CHECK_CLOSE(0.0, recruiter.getClosingRate(), 0.001);

    recruiter.registerCandidate();
    recruiter.registerCandidate();
    CHECK_EQUAL(2, recruiter.getCandidatesInProcess());
    CHECK_CLOSE(0.0, recruiter.getClosingRate(), 0.001);

    recruiter.closeVacancy();
    CHECK_EQUAL(1, recruiter.getVacanciesClosed());
    CHECK_EQUAL(1, recruiter.getCandidatesInProcess());
    CHECK_CLOSE(0.5, recruiter.getClosingRate(), 0.001);
}

TEST(WorkerFactoryTest_CreatesEveryRoleWithCorrectPosition) {
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
        CHECK(worker != nullptr);
        CHECK(!worker->getRole().empty());
        CHECK(!worker->getPosition().empty());
        roles.push_back(worker->getRole());
    }

    CHECK_EQUAL(12u, roles.size());
    std::sort(roles.begin(), roles.end());
    CHECK(std::adjacent_find(roles.begin(), roles.end()) == roles.end());
}

TEST(WorkerFactoryTest_RejectsMissingTermsOfEmployment) {
    CHECK_THROW(
        WorkerFactory::create(1, "Без контракта", WorkerType::Accountant, nullptr, makeAdvance()),
        InvalidInputException);
    CHECK_THROW(
        WorkerFactory::create(1, "Без аванса", WorkerType::Accountant, makeContract(), nullptr),
        InvalidInputException);
}

TEST(CompanyTest_RejectsEmptyNameAndNullDepartment) {
    CHECK_THROW(Company(""), InvalidInputException);
    Company c("Acme");
    CHECK_THROW(c.addDepartment(nullptr), InvalidInputException);
}

TEST(CompanyTest_AppointRecruiterBootstrapsHiring) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    const Worker* recruiter = company.findDepartment("Отдел кадров")->findWorker(1);
    CHECK(recruiter != nullptr);
    CHECK_EQUAL("Recruiter", recruiter->getRole());

    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);
    CHECK(developer != nullptr);
    CHECK_EQUAL("Разработчик ПО", developer->getPosition());
}

TEST(CompanyTest_HireAssignsSequentialIdsAndContractNumber) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    Worker* first = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                       kDeveloperRate);
    Worker* second =
        company.hireWorker("IT-отдел", "Сидоров С.С.", WorkerType::SystemAdministrator, 140000.0);

    CHECK(first != nullptr);
    CHECK(second != nullptr);
    CHECK(first->getId() != second->getId());
    CHECK_EQUAL(Recruiter::contractNumberFor(first->getId()),
                first->getContract().getContractNumber());
    CHECK_EQUAL(3, company.getTotalWorkersCount());
}

TEST(CompanyTest_HiredWorkerBelongsToDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);

    CHECK(developer != nullptr);
    Department* it = company.findDepartment("IT-отдел");
    CHECK(it != nullptr);
    CHECK(developer->worksIn(it));
    CHECK(it->hasWorker(developer->getId()));
}

TEST(CompanyTest_HireRejectsUnknownDepartmentAndDuplicateName) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);

    CHECK_THROW(company.hireWorker("Юридический", "Петров П.П.", WorkerType::Lawyer, 130000.0),
                DepartmentNotFoundException);
    CHECK_THROW(company.hireWorker("IT-отдел", "", WorkerType::SoftwareDeveloper, kDeveloperRate),
                InvalidInputException);
    CHECK_THROW(company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, 0.0),
                InvalidInputException);
}

TEST(CompanyTest_TerminateWorkerRemovesFromDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    Worker* developer = company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper,
                                           kDeveloperRate);
    CHECK(developer != nullptr);
    const int id = developer->getId();

    std::unique_ptr<Worker> terminated = company.terminateWorker("IT-отдел", id);
    CHECK(terminated != nullptr);
    CHECK(!company.findDepartment("IT-отдел")->hasWorker(id));
    CHECK(!terminated->getContract().isActiveOn(today()));

    CHECK_THROW(company.terminateWorker("IT-отдел", id), EmployeeNotFoundException);
    CHECK_THROW(company.terminateWorker("Нет такого", id), DepartmentNotFoundException);
}

TEST(CompanyTest_PayrollSumsUpEveryDepartment) {
    Company company{"Acme LLC"};
    populateReadyCompany(company);
    company.hireWorker("IT-отдел", "Петров П.П.", WorkerType::SoftwareDeveloper, kDeveloperRate);
    company.hireWorker("Производство", "Сидоров С.С.", WorkerType::Technician, 85000.0);

    const double payroll = company.calculateCompanyPayroll();
    const double itPayroll = company.findDepartment("IT-отдел")->calculatePayroll();

    CHECK(payroll > 0.0);
    CHECK(itPayroll > 0.0);
    CHECK(itPayroll <= payroll);
    const std::string report = company.generateReport();
    CHECK(report.find("Acme LLC") != std::string::npos);
    CHECK(report.find("ФОТ") != std::string::npos);
    CHECK(report.find("сотрудников: 3") != std::string::npos);
}
