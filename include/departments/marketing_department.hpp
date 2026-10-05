#pragma once

#include "department.hpp"

class MarketingDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 30;
    static constexpr double kMonthlyBudget = 4500000;

    MarketingDepartment();
};
