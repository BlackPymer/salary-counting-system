#pragma once

#include "financial_objects/salary.hpp"
#include "payroll/calculation_context.hpp"

class DeductionStrategy {
public:
    virtual ~DeductionStrategy() = default;
    virtual void apply(const CalculationContext& context, Salary& salary) const = 0;
};
