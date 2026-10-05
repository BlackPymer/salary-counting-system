#pragma once

#include "department.hpp"

class SecurityDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 45;
    static constexpr double kMonthlyBudget = 3500000;

    SecurityDepartment();
};
