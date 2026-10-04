#include "financial_objects/salary.hpp"

#include <iomanip>
#include <sstream>

#include "exceptions/invalid_input_exception.hpp"

Salary::Salary(double gross, double taxDeduction, double net)
    : gross_(gross), taxDeduction_(taxDeduction), net_(net) {}

double Salary::getGross() const {
    return gross_;
}

double Salary::getTaxDeduction() const {
    return taxDeduction_;
}

double Salary::getNet() const {
    return net_;
}

void Salary::applyBonus(double amount) {
    if (amount < 0.0) {
        throw InvalidInputException("Надбавка не может быть отрицательной");
    }
    gross_ += amount;
    net_ += amount;
}

void Salary::applyDeduction(double amount) {
    if (amount < 0.0) {
        throw InvalidInputException("Удержание не может быть отрицательным");
    }
    gross_ -= amount;
    net_ -= amount;
}

void Salary::applyRepayment(double amount) {
    if (amount < 0.0) {
        throw InvalidInputException("Сумма зачёта не может быть отрицательной");
    }
    if (amount > net_) {
        throw InvalidInputException("Зачёт превышает сумму на руки");
    }
    net_ -= amount;
}

void Salary::applyTax(double taxAmount) {
    if (taxAmount < 0.0) {
        throw InvalidInputException("Налог не может быть отрицательным");
    }
    if (taxAmount > net_) {
        throw InvalidInputException("Налог превышает сумму на руки");
    }
    taxDeduction_ += taxAmount;
    net_ -= taxAmount;
}

std::string Salary::toString() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << "gross=" << gross_ << " tax=" << taxDeduction_
        << " net=" << net_;
    return out.str();
}
