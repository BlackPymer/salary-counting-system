#include "payroll/strategies/tax_strategy.hpp"

void TaxStrategy::apply(const CalculationContext&, Salary& salary) const {
    const double gross = salary.getGross();
    if (gross <= 0.0) {
        return;
    }
    double tax = 0.0;
    if (gross <= kBracket1) {
        tax = gross * kRate13;
    } else if (gross <= kBracket2) {
        tax = kBracket1 * kRate13 + (gross - kBracket1) * kRate15;
    } else if (gross <= kBracket3) {
        tax = kBracket1 * kRate13 + kBracket1 * kRate15 + (gross - kBracket2) * kRate18;
    } else if (gross <= kBracket4) {
        tax = kBracket1 * kRate13 + kBracket1 * kRate15 + kBracket1 * kRate18 +
              (gross - kBracket3) * kRate20;
    } else {
        tax = kBracket1 * kRate13 + kBracket1 * kRate15 + kBracket1 * kRate18 +
              kBracket1 * kRate20 + (gross - kBracket4) * kRate22;
    }
    if (tax > 0.0) {
        salary.applyTax(tax);
    }
}
