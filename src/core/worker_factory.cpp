#include "core/worker_factory.hpp"

#include "departments/accounting/workers/accountant.hpp"
#include "departments/accounting/workers/payroll_accountant.hpp"
#include "departments/administration/workers/administrator.hpp"
#include "departments/customer_support/workers/customer_support_agent.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/it/workers/software_developer.hpp"
#include "departments/it/workers/system_administrator.hpp"
#include "departments/legal/workers/lawyer.hpp"
#include "departments/marketing/workers/marketer.hpp"
#include "departments/production/workers/technician.hpp"
#include "departments/research_n_developement/workers/research_scientist.hpp"
#include "departments/security/workers/security_guard.hpp"
#include "exceptions/invalid_input_exception.hpp"

std::string WorkerFactory::positionFor(WorkerType type) {
    switch (type) {
        case WorkerType::Accountant:
            return "Бухгалтер";
        case WorkerType::PayrollAccountant:
            return "Бухгалтер по заработной плате";
        case WorkerType::Administrator:
            return "Администратор";
        case WorkerType::CustomerSupportAgent:
            return "Оператор клиентской поддержки";
        case WorkerType::Recruiter:
            return "Рекрутер";
        case WorkerType::SoftwareDeveloper:
            return "Разработчик ПО";
        case WorkerType::SystemAdministrator:
            return "Системный администратор";
        case WorkerType::Lawyer:
            return "Юрист";
        case WorkerType::Marketer:
            return "Маркетолог";
        case WorkerType::Technician:
            return "Техник";
        case WorkerType::ResearchScientist:
            return "Научный сотрудник";
        case WorkerType::SecurityGuard:
            return "Охранник";
    }
    throw InvalidInputException("Неизвестный тип сотрудника");
}

std::unique_ptr<Worker> WorkerFactory::create(int id, const std::string& fullName, WorkerType type,
                                              std::unique_ptr<EmploymentContract> contract,
                                              std::unique_ptr<AdvancePayment> advance) {
    if (contract == nullptr || advance == nullptr) {
        throw InvalidInputException("Сотрудник не может быть создан без контракта и аванса");
    }

    switch (type) {
        case WorkerType::Accountant:
            return std::make_unique<Accountant>(id, fullName, std::move(contract),
                                                std::move(advance), "junior");
        case WorkerType::PayrollAccountant:
            return std::make_unique<PayrollAccountant>(id, fullName, std::move(contract),
                                                       std::move(advance), "junior", 0);
        case WorkerType::Administrator:
            return std::make_unique<Administrator>(id, fullName, std::move(contract),
                                                   std::move(advance), "B-1");
        case WorkerType::CustomerSupportAgent:
            return std::make_unique<CustomerSupportAgent>(id, fullName, std::move(contract),
                                                          std::move(advance), "chat");
        case WorkerType::Recruiter:
            return std::make_unique<Recruiter>(id, fullName, std::move(contract),
                                               std::move(advance));
        case WorkerType::SoftwareDeveloper:
            return std::make_unique<SoftwareDeveloper>(id, fullName, std::move(contract),
                                                       std::move(advance), "C++");
        case WorkerType::SystemAdministrator:
            return std::make_unique<SystemAdministrator>(id, fullName, std::move(contract),
                                                         std::move(advance), 0);
        case WorkerType::Lawyer:
            return std::make_unique<Lawyer>(id, fullName, std::move(contract), std::move(advance),
                                            "pending");
        case WorkerType::Marketer:
            return std::make_unique<Marketer>(id, fullName, std::move(contract),
                                              std::move(advance));
        case WorkerType::Technician:
            return std::make_unique<Technician>(id, fullName, std::move(contract),
                                                std::move(advance),
                                                std::vector<std::string>{"default"});
        case WorkerType::ResearchScientist:
            return std::make_unique<ResearchScientist>(id, fullName, std::move(contract),
                                                       std::move(advance), "general");
        case WorkerType::SecurityGuard:
            return std::make_unique<SecurityGuard>(id, fullName, std::move(contract),
                                                   std::move(advance), "day");
    }
    throw InvalidInputException("Неизвестный тип сотрудника");
}
