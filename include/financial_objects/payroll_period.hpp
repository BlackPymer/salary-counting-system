#pragma once

#include <chrono>
#include <string>

#include "core/date.hpp"

class PayrollPeriod {
public:
    PayrollPeriod();
    PayrollPeriod(int year, int month);

    int getYear() const;
    int getMonth() const;

    Date getFirstDay() const;
    Date getLastDay() const;

    bool contains(const Date& date) const;
    int getDaysInMonth() const;

    std::string toString() const;

private:
    int year_ = 0;
    int month_ = 0;
    int daysInMonth_ = 0;
};
