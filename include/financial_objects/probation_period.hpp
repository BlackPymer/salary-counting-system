#pragma once

#include "core/date.hpp"

class ProbationPeriod {
public:
    static constexpr int DEFAULT_DURATION_DAYS = 30;
    static constexpr double DISCOUNT_RATE = 0.15;

    ProbationPeriod() = default;
    explicit ProbationPeriod(Date startDate, int durationDays = DEFAULT_DURATION_DAYS);

    bool isActiveOn(const Date& date) const;
    bool isCompleted() const;
    int daysRemaining(const Date& date) const;
    int getDurationDays() const;
    double getDiscountRate() const;

    double calculateRate(double baseRate) const;

private:
    Date startDate_;
    int durationDays_ = DEFAULT_DURATION_DAYS;
};
