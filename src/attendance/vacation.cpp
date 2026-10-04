#include "attendance/vacation.hpp"

Vacation::Vacation(int days, bool isPaid) : Absence(days, isPaid ? kFullPay : kNoPay) {}

std::unique_ptr<Vacation> Vacation::paid(int days) {
    return std::unique_ptr<Vacation>(new Vacation(days, true));
}

std::unique_ptr<Vacation> Vacation::unpaid(int days) {
    return std::unique_ptr<Vacation>(new Vacation(days, false));
}

std::string Vacation::getTypeName() const {
    return getPayRate() == kFullPay ? "Отпуск" : "Отпуск без сохранения з/п";
}
