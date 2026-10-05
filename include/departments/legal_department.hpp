#pragma once

#include "department.hpp"

class LegalDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 15;
    static constexpr double kMonthlyBudget = 2500000;

    LegalDepartment();
};
