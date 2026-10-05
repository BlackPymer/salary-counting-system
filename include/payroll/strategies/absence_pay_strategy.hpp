#pragma once

#include "payroll/strategies/deduction_strategy.hpp"

class AbsencePayStrategy : public DeductionStrategy {
public:
    void apply(const CalculationContext& context, Salary& salary) const override;
};
