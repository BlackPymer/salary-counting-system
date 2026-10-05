#include "departments/accounting_department.hpp"

AccountingDepartment::AccountingDepartment()
    : Department("Бухгалтерия", "Расчёт заработной платы, налогов и отчётности", kMaxHeadcount,
                 kMonthlyBudget) {}
