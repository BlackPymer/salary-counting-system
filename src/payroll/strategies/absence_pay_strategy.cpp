#include "payroll/strategies/absence_pay_strategy.hpp"

void AbsencePayStrategy::apply(const CalculationContext& context, Salary& salary) const {
    const auto& absences = context.getAbsences();
    if (absences.empty()) {
        return;
    }
    const double dailyRate = context.getBaseRate() / 30.0;
    double deduction = 0.0;
    for (const auto& absence : absences) {
        deduction += dailyRate * absence->getDays() * (1.0 - absence->getPayRate());
    }
    if (deduction > 0.0) {
        salary.applyDeduction(deduction);
    }
}
