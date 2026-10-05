#include "payroll/strategies/tax_strategy.hpp"

void TaxStrategy::apply(const CalculationContext& /*context*/, Salary& salary) const {
    const double gross = salary.getGross();
    if (gross <= 0.0) {
        return;
    }
    double tax = 0.0;
    if (gross <= 200000.0) {
        tax = gross * 0.13;
    } else if (gross <= 400000.0) {
        tax = 200000.0 * 0.13 + (gross - 200000.0) * 0.15;
    } else if (gross <= 600000.0) {
        tax = 200000.0 * 0.13 + 200000.0 * 0.15 + (gross - 400000.0) * 0.18;
    } else if (gross <= 800000.0) {
        tax = 200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + (gross - 600000.0) * 0.20;
    } else {
        tax = 200000.0 * 0.13 + 200000.0 * 0.15 + 200000.0 * 0.18 + 200000.0 * 0.20 +
              (gross - 800000.0) * 0.22;
    }
    if (tax > 0.0) {
        salary.applyTax(tax);
    }
}
