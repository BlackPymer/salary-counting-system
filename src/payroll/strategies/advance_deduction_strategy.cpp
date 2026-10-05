#include "payroll/strategies/advance_deduction_strategy.hpp"

void AdvanceDeductionStrategy::apply(const CalculationContext& context, Salary& salary) const {
    auto& advance = const_cast<AdvancePayment&>(context.getAdvance());
    const double repaid = advance.applyDeduction(salary.getNet());
    if (repaid > 0.0) {
        salary.applyRepayment(repaid);
    }
}
