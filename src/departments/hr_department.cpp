#include "departments/hr_department.hpp"

HrDepartment::HrDepartment()
    : Department(kName, "Найм, адаптация и кадровое делопроизводство", kMaxHeadcount,
                 kMonthlyBudget) {}
