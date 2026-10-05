#pragma once

#include "department.hpp"

class HrDepartment : public Department {
public:
    static constexpr int kMaxHeadcount = 30;
    static constexpr double kMonthlyBudget = 3000000;

    static constexpr const char* kName = "Отдел кадров";

    HrDepartment();
};
