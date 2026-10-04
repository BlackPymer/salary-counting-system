#include "attendance/absence.hpp"

#include "exceptions/invalid_input_exception.hpp"

Absence::Absence(int days, double payRate) : days_(days), payRate_(payRate) {
    if (days <= 0) {
        throw InvalidInputException("Количество дней отсутствия должно быть положительным");
    }
    if (payRate < 0.0 || payRate > 1.0) {
        throw InvalidInputException("Ставка оплаты отсутствия должна быть в диапазоне [0, 1]");
    }
}

std::unique_ptr<Absence> Absence::dayOff(int days) {
    return std::unique_ptr<Absence>(new Absence(days, kHalfPay));
}

int Absence::getDays() const {
    return days_;
}

double Absence::getPayRate() const {
    return payRate_;
}

std::string Absence::getTypeName() const {
    return "Отсутствие";
}
