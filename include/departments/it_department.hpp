#pragma once

#include "department.hpp"

class ItDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 60;
    static constexpr double kMonthlyBudget = 8000000;

    ItDepartment();
};
