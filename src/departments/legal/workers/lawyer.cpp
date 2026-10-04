#include "departments/legal/workers/lawyer.hpp"

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kBaseLawyerRate = 0.13;
constexpr double kCaseloadBonus = 0.07;
constexpr int kCaseloadThreshold = 5;
}  // namespace

Lawyer::Lawyer(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
               std::unique_ptr<AdvancePayment> advance, std::string barNumber)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      barNumber_(std::move(barNumber)) {
    if (barNumber_.empty()) {
        throw InvalidInputException("Номер адвокатского удостоверения обязателен");
    }
}

std::string Lawyer::getRole() const {
    return "Lawyer";
}

std::string Lawyer::getSpecialization() const {
    return "Юридическое сопровождение, № " + barNumber_;
}

const std::string& Lawyer::getBarNumber() const {
    return barNumber_;
}

int Lawyer::getCasesHandled() const {
    return casesHandled_;
}

void Lawyer::handleCase() {
    ++casesHandled_;
}

bool Lawyer::reviewsEmploymentContracts() const {
    return true;
}

double Lawyer::calculateRoleBonus() const {
    return getBaseRate() *
           (kBaseLawyerRate + (casesHandled_ >= kCaseloadThreshold ? kCaseloadBonus : 0.0));
}
