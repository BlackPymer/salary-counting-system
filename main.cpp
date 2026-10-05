#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "attendance/absence.hpp"
#include "attendance/sick_leave.hpp"
#include "attendance/vacation.hpp"
#include "company.hpp"
#include "departments/accounting_department.hpp"
#include "departments/administration_department.hpp"
#include "departments/customer_support_department.hpp"
#include "departments/hr_department.hpp"
#include "departments/it_department.hpp"
#include "departments/legal_department.hpp"
#include "departments/marketing_department.hpp"
#include "departments/production_department.hpp"
#include "departments/research_n_developement_department.hpp"
#include "departments/security_department.hpp"
#include "exceptions/base_exception.hpp"

namespace {

void printMenu() {
    std::cout << "\n=== Меню компании ===\n"
              << " 1. Пополнить счёт\n"
              << " 2. Нанять сотрудника\n"
              << " 3. Уволить сотрудника\n"
              << " 4. Зарегистрировать отсутствие\n"
              << " 5. Зарегистрировать сверхурочные\n"
              << " 6. Завершить месяц (выплаты)\n"
              << " 7. Отчёт по компании\n"
              << " 8. Список сотрудников\n"
              << " 0. Выход\n"
              << "Выбор: ";
}

void printDepartments(const Company& company) {
    std::cout << "Отделы:\n";
    static const char* names[] = {
        "Отдел кадров",      "IT-отдел",  "Бухгалтерия",  "Администрация", "Клиентская поддержка",
        "Юридический отдел", "Маркетинг", "Производство", "НИОКР",         "Служба безопасности"};
    for (const char* name : names) {
        if (company.findDepartment(name) != nullptr) {
            std::cout << "  - " << name << '\n';
        }
    }
}

void printWorkerTypes() {
    std::cout << "Типы сотрудников:\n"
              << " 1. Бухгалтер\n"
              << " 2. Бухгалтер по ЗП\n"
              << " 3. Администратор\n"
              << " 4. Оператор поддержки\n"
              << " 5. Рекрутер\n"
              << " 6. Разработчик ПО\n"
              << " 7. Сисадмин\n"
              << " 8. Юрист\n"
              << " 9. Маркетолог\n"
              << "10. Техник\n"
              << "11. Научный сотрудник\n"
              << "12. Охранник\n"
              << "Тип: ";
}

WorkerType workerTypeFromInt(int n) {
    switch (n) {
        case 1:
            return WorkerType::Accountant;
        case 2:
            return WorkerType::PayrollAccountant;
        case 3:
            return WorkerType::Administrator;
        case 4:
            return WorkerType::CustomerSupportAgent;
        case 5:
            return WorkerType::Recruiter;
        case 6:
            return WorkerType::SoftwareDeveloper;
        case 7:
            return WorkerType::SystemAdministrator;
        case 8:
            return WorkerType::Lawyer;
        case 9:
            return WorkerType::Marketer;
        case 10:
            return WorkerType::Technician;
        case 11:
            return WorkerType::ResearchScientist;
        case 12:
            return WorkerType::SecurityGuard;
        default:
            return WorkerType::SoftwareDeveloper;
    }
}

void printAbsenceTypes() {
    std::cout << "Тип отсутствия:\n"
              << " 1. Оплачиваемый отпуск\n"
              << " 2. Неоплачиваемый отпуск\n"
              << " 3. Больничный\n"
              << " 4. Отгул\n"
              << "Тип: ";
}

void listWorkers(Company& company) {
    static const char* names[] = {
        "Отдел кадров",      "IT-отдел",  "Бухгалтерия",  "Администрация", "Клиентская поддержка",
        "Юридический отдел", "Маркетинг", "Производство", "НИОКР",         "Служба безопасности"};
    for (const char* name : names) {
        Department* dept = company.findDepartment(name);
        if (dept == nullptr) {
            continue;
        }
        const auto workers = dept->getWorkers();
        if (workers.empty()) {
            continue;
        }
        std::cout << name << ":\n";
        for (const Worker* w : workers) {
            std::cout << "  [" << w->getId() << "] " << w->getFullName() << " — "
                      << w->getPosition() << " (ставка " << w->getBaseRate() << ")\n";
        }
    }
}

Company createCompany() {
    Company company{"Симуля-ЛТД"};
    company.addDepartment(std::make_unique<HrDepartment>());
    company.addDepartment(std::make_unique<ItDepartment>());
    company.addDepartment(std::make_unique<AccountingDepartment>());
    company.addDepartment(std::make_unique<AdministrationDepartment>());
    company.addDepartment(std::make_unique<CustomerSupportDepartment>());
    company.addDepartment(std::make_unique<LegalDepartment>());
    company.addDepartment(std::make_unique<MarketingDepartment>());
    company.addDepartment(std::make_unique<ProductionDepartment>());
    company.addDepartment(std::make_unique<ResearchNDevelopementDepartment>());
    company.addDepartment(std::make_unique<SecurityDepartment>());
    company.appointRecruiter("Иванова М.С.", 95000.0);
    return company;
}

}  // namespace

int main() {
    Company company = createCompany();
    std::cout << "Компания создана. Рекрутер назначен.\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = 0;
        if (!(std::cin >> choice)) {
            break;
        }

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Сумма пополнения: ";
                    double amount = 0.0;
                    std::cin >> amount;
                    company.addFunds(amount);
                    std::cout << "Баланс: " << company.getBalance() << '\n';
                    break;
                }
                case 2: {
                    printDepartments(company);
                    std::cout << "Отдел: ";
                    std::string deptName;
                    std::cin.ignore();
                    std::getline(std::cin, deptName);
                    printWorkerTypes();
                    int typeNum = 0;
                    std::cin >> typeNum;
                    std::cout << "ФИО: ";
                    std::string fullName;
                    std::cin.ignore();
                    std::getline(std::cin, fullName);
                    std::cout << "Ставка: ";
                    double rate = 0.0;
                    std::cin >> rate;
                    Worker* w =
                        company.hireWorker(deptName, fullName, workerTypeFromInt(typeNum), rate);
                    std::cout << "Нанят: [" << w->getId() << "] " << w->getFullName() << '\n';
                    break;
                }
                case 3: {
                    listWorkers(company);
                    std::cout << "Отдел: ";
                    std::string deptName;
                    std::cin.ignore();
                    std::getline(std::cin, deptName);
                    std::cout << "ID: ";
                    int id = 0;
                    std::cin >> id;
                    auto fired = company.terminateWorker(deptName, id);
                    std::cout << "Уволен: " << fired->getFullName() << '\n';
                    break;
                }
                case 4: {
                    listWorkers(company);
                    std::cout << "Отдел: ";
                    std::string deptName;
                    std::cin.ignore();
                    std::getline(std::cin, deptName);
                    std::cout << "ID сотрудника: ";
                    int id = 0;
                    std::cin >> id;
                    printAbsenceTypes();
                    int type = 0;
                    std::cin >> type;
                    std::cout << "Дней: ";
                    int days = 0;
                    std::cin >> days;
                    std::unique_ptr<Absence> absence;
                    if (type == 1) {
                        absence = Vacation::paid(days);
                    } else if (type == 2) {
                        absence = Vacation::unpaid(days);
                    } else if (type == 3) {
                        std::cout << "Стаж (лет): ";
                        int seniority = 0;
                        std::cin >> seniority;
                        absence = SickLeave::forDays(days, seniority);
                    } else {
                        absence = Absence::dayOff(days);
                    }
                    company.registerAbsence(deptName, id, std::move(absence));
                    std::cout << "Отсутствие зарегистрировано.\n";
                    break;
                }
                case 5: {
                    listWorkers(company);
                    std::cout << "Отдел: ";
                    std::string deptName;
                    std::cin.ignore();
                    std::getline(std::cin, deptName);
                    std::cout << "ID сотрудника: ";
                    int id = 0;
                    std::cin >> id;
                    std::cout << "Часов: ";
                    double hours = 0.0;
                    std::cin >> hours;
                    company.registerOvertime(deptName, id, hours);
                    std::cout << "Сверхурочные зарегистрированы.\n";
                    break;
                }
                case 6: {
                    const double payroll = company.calculateCompanyPayroll();
                    std::cout << "ФОТ: " << payroll << ", баланс: " << company.getBalance() << '\n';
                    company.endPeriod();
                    std::cout << "Месяц завершён. Выплаты проведены.\n"
                              << "Баланс: " << company.getBalance() << '\n';
                    break;
                }
                case 7: {
                    std::cout << company.generateReport();
                    std::cout << "Баланс: " << company.getBalance() << '\n';
                    break;
                }
                case 8: {
                    listWorkers(company);
                    break;
                }
                case 0: {
                    running = false;
                    break;
                }
                default:
                    std::cout << "Неизвестная команда.\n";
            }
        } catch (const BaseException& e) {
            std::cerr << "Ошибка: " << e.what() << '\n';
        }
    }

    std::cout << "До свидания.\n";
    return 0;
}
