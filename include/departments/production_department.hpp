#pragma once

#include "department.hpp"

class ProductionDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 120;
    static constexpr double kMonthlyBudget = 12000000;

    ProductionDepartment();
};
