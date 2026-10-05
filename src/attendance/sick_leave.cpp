#include "attendance/sick_leave.hpp"

SickLeave::SickLeave(int days, int seniorityYears)
    : Absence(days, kFullPay), seniorityYears_(seniorityYears) {
    if (seniorityYears < 0) {
        seniorityYears_ = 0;
    }
}

std::unique_ptr<SickLeave> SickLeave::forDays(int days, int seniorityYears) {
    return std::unique_ptr<SickLeave>(new SickLeave(days, seniorityYears));
}

int SickLeave::getSeniorityYears() const {
    return seniorityYears_;
}

double SickLeave::getPayRate() const {
    if (seniorityYears_ < kJuniorThresholdYears) {
        return kJuniorPayRate;
    }
    if (seniorityYears_ < kMiddleThresholdYears) {
        return kMiddlePayRate;
    }
    return kFullPay;
}

std::string SickLeave::getTypeName() const {
    return "Больничный (стаж " + std::to_string(seniorityYears_) + " лет, выплата " +
           std::to_string(static_cast<int>(getPayRate() * 100)) + "%)";
}
