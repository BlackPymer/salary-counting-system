#include "financial_objects/probation_period.hpp"
#include "exceptions/invalid_input_exception.hpp"

ProbationPeriod::ProbationPeriod(Date startDate, int durationDays)
    : startDate_(startDate), durationDays_(durationDays) {
    if (durationDays <= 0) {
        throw InvalidInputException("Длительность испытательного срока должна быть положительной");
    }
}

bool ProbationPeriod::isActiveOn(const Date& date) const {
    return !isEarlier(date, startDate_) && daysBetween(startDate_, date) < durationDays_;
}

bool ProbationPeriod::isCompleted() const {
    return isActiveOn(today());
}

int ProbationPeriod::daysRemaining(const Date& date) const {
    if (isEarlier(date, startDate_)) {
        return durationDays_;
    }
    const int elapsed = daysBetween(startDate_, date);
    if (elapsed >= durationDays_) {
        return 0;
    }
    return durationDays_ - elapsed;
}

int ProbationPeriod::getDurationDays() const {
    return durationDays_;
}

double ProbationPeriod::getDiscountRate() const {
    return DISCOUNT_RATE;
}

double ProbationPeriod::calculateRate(double baseRate) const {
    return baseRate * (1.0 - DISCOUNT_RATE);
}
