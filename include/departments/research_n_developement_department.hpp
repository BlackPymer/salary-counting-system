#pragma once

#include "department.hpp"

class ResearchNDevelopementDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 40;
    static constexpr double kMonthlyBudget = 6000000;

    ResearchNDevelopementDepartment();
};
