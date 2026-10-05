#pragma once

#include "payroll/strategies/deduction_strategy.hpp"

class TaxStrategy : public DeductionStrategy {
public:
    static constexpr double kBracket1 = 200000.0;
    static constexpr double kBracket2 = 400000.0;
    static constexpr double kBracket3 = 600000.0;
    static constexpr double kBracket4 = 800000.0;
    static constexpr double kRate13 = 0.13;
    static constexpr double kRate15 = 0.15;
    static constexpr double kRate18 = 0.18;
    static constexpr double kRate20 = 0.20;
    static constexpr double kRate22 = 0.22;

    void apply(const CalculationContext& context, Salary& salary) const override;
};
