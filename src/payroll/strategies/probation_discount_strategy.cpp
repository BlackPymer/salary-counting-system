#include "payroll/strategies/probation_discount_strategy.hpp"

void ProbationDiscountStrategy::apply(const CalculationContext& context, Salary& salary) const {
    if (context.isOnProbation()) {
        const double discount = salary.getGross() * 0.15;
        if (discount > 0.0) {
            salary.applyDeduction(discount);
        }
    }
}
