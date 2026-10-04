#include "departments/accounting_department.hpp"

AccountingDepartment::AccountingDepartment() : Department("Бухгалтерия") {}

std::string AccountingDepartment::getDescription() const {
    return "Расчёт заработной платы, налогов и отчётности";
}
