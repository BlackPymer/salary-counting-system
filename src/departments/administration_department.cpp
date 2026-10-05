#include "departments/administration_department.hpp"

AdministrationDepartment::AdministrationDepartment()
    : Department("Администрация", "Управление хозяйственной деятельностью офиса", kMaxHeadcount,
                 kMonthlyBudget) {}
