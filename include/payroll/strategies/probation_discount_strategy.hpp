#pragma once

#include "payroll/strategies/deduction_strategy.hpp"

class ProbationDiscountStrategy : public DeductionStrategy {
public:
    static constexpr double kProbationDiscount = 0.15;

    void apply(const CalculationContext& context, Salary& salary) const override;
};
