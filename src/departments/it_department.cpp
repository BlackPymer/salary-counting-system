#include "departments/it_department.hpp"

ItDepartment::ItDepartment()
    : Department("IT-отдел", "Разработка ПО, инфраструктура и техническая поддержка", kMaxHeadcount,
                 kMonthlyBudget) {}
