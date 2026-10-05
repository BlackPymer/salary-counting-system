#pragma once

#include "department.hpp"

class AccountingDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 25;
    static constexpr double kMonthlyBudget = 3000000;

    AccountingDepartment();
};
