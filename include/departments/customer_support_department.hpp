#pragma once

#include "department.hpp"

class CustomerSupportDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 50;
    static constexpr double kMonthlyBudget = 4000000;

    CustomerSupportDepartment();
};
