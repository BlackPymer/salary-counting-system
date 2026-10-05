#pragma once

#include "payroll/strategies/deduction_strategy.hpp"

class AdvanceDeductionStrategy : public DeductionStrategy {
public:
    void apply(const CalculationContext& context, Salary& salary) const override;
};
