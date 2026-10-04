#pragma once

#include <string>

// Результат расчёта зарплаты за период: gross, налог и сумма на руки.
class Salary {
public:
    Salary() = default;
    Salary(double gross, double taxDeduction, double net);

    double getGross() const;
    double getTaxDeduction() const;
    double getNet() const;

    void applyBonus(double amount);
    void applyDeduction(double amount);
    void applyTax(double taxAmount);

    std::string toString() const;

private:
    double gross_ = 0.0;
    double taxDeduction_ = 0.0;
    double net_ = 0.0;
};
