#pragma once

#include "payroll/strategies/deduction_strategy.hpp"

class AbsencePayStrategy : public DeductionStrategy {
public:
    static constexpr double kDaysPerMonth = 30.0;

    void apply(const CalculationContext& context, Salary& salary) const override;
};
