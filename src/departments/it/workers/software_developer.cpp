#include "departments/it/workers/software_developer.hpp"

namespace {
constexpr double kBaseDeveloperRate = 0.12;
constexpr double kReviewBonus = 0.08;
}  // namespace

SoftwareDeveloper::SoftwareDeveloper(int id, std::string fullName,
                                     std::unique_ptr<EmploymentContract> contract,
                                     std::unique_ptr<AdvancePayment> advance, std::string primaryLanguage)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      primaryLanguage_(std::move(primaryLanguage)) {}

std::string SoftwareDeveloper::getRole() const {
    return "SoftwareDeveloper";
}

std::string SoftwareDeveloper::getSpecialization() const {
    return "Разработка ПО (" + primaryLanguage_ + ")";
}

const std::string& SoftwareDeveloper::getPrimaryLanguage() const {
    return primaryLanguage_;
}

int SoftwareDeveloper::getTasksCompleted() const {
    return tasksCompleted_;
}

int SoftwareDeveloper::getCodeReviewsGiven() const {
    return codeReviewsGiven_;
}

void SoftwareDeveloper::completeTask() {
    ++tasksCompleted_;
}

void SoftwareDeveloper::reviewCode() {
    ++codeReviewsGiven_;
}

bool SoftwareDeveloper::isFullCycle() const {
    return tasksCompleted_ > 0 && codeReviewsGiven_ > 0;
}

double SoftwareDeveloper::calculateRoleBonus() const {
    return getBaseRate() * (kBaseDeveloperRate + (isFullCycle() ? kReviewBonus : 0.0));
}
