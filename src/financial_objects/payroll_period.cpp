#include "financial_objects/payroll_period.hpp"

#include <sstream>

#include "exceptions/invalid_input_exception.hpp"

PayrollPeriod::PayrollPeriod() {
    const auto now = std::chrono::system_clock::now();
    const auto ymd = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(now));
    year_ = static_cast<int>(ymd.year());
    month_ = static_cast<int>(static_cast<unsigned int>(ymd.month()));
}

PayrollPeriod::PayrollPeriod(int year, int month) : year_(year), month_(month) {
    if (month_ < 1 || month_ > 12) {
        throw InvalidInputException("Месяц должен быть от 1 до 12");
    }
    if (year_ < 1) {
        throw InvalidInputException("Год должен быть положительным");
    }
}

int PayrollPeriod::getYear() const {
    return year_;
}

int PayrollPeriod::getMonth() const {
    return month_;
}

Date PayrollPeriod::getFirstDay() const {
    return Date(std::chrono::year{year_}, std::chrono::month{static_cast<unsigned int>(month_)},
                std::chrono::day{1});
}

Date PayrollPeriod::getLastDay() const {
    const std::chrono::year_month ym(std::chrono::year{year_},
                                     std::chrono::month{static_cast<unsigned int>(month_)});
    const std::chrono::year_month_day_last ymdl(std::chrono::year{year_},
                                                std::chrono::month_day_last{ym.month()});
    const auto last_day = ymdl.day();
    return Date(std::chrono::year{year_}, ym.month(), last_day);
}

bool PayrollPeriod::contains(const Date& date) const {
    if (!date.ok()) {
        return false;
    }
    if (static_cast<int>(date.year()) != year_) {
        return false;
    }
    if (static_cast<unsigned int>(date.month()) != static_cast<unsigned int>(month_)) {
        return false;
    }
    return true;
}

std::string PayrollPeriod::toString() const {
    std::ostringstream out;
    out << year_ << "-" << (month_ < 10 ? "0" : "") << month_;
    return out.str();
}
