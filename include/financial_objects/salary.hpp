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

    // Надбавка увеличивает начисленный доход.
    void applyBonus(double amount);

    // Удержание из начислений: уменьшает и gross, и net.
    // Применяется к штрафам и неотработанным дням.
    void applyDeduction(double amount);

    // Зачёт аванса: уменьшает только net, gross остаётся начисленным.
    void applyRepayment(double amount);

    void applyTax(double taxAmount);

    std::string toString() const;

private:
    double gross_ = 0.0;
    double taxDeduction_ = 0.0;
    double net_ = 0.0;
};
