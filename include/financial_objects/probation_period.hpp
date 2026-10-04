#pragma once

#include "core/date.hpp"

// Испытательный срок при найме: ставка ниже на 15%.
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

    // Ставка с учётом скидки, пока сотрудник на испытательном.
    double calculateRate(double baseRate) const;

private:
    Date startDate_;
    int durationDays_ = DEFAULT_DURATION_DAYS;
};
