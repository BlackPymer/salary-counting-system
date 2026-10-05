#pragma once

#include "department.hpp"

class AdministrationDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 20;
    static constexpr double kMonthlyBudget = 2500000;

    AdministrationDepartment();
};
