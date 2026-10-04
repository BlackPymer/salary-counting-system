#include "attendance/sick_leave.hpp"

namespace {
// Выплата по больничному зависит от стажа: меньше 3 лет — 60%,
// до 5 лет — 80%, 5 лет и больше — 100%.
constexpr int kJuniorThresholdYears = 3;
constexpr int kMiddleThresholdYears = 5;
constexpr double kJuniorPayRate = 0.6;
constexpr double kMiddlePayRate = 0.8;
}  // namespace

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
