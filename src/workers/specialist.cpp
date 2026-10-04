#include "workers/specialist.hpp"

std::string Specialist::getRole() const {
    return "Specialist";
}

std::string Specialist::getSpecialization() const {
    return getPosition();
}

double Specialist::getSpecializationBonusRate() const {
    return specializationBonusRate_;
}

double Specialist::calculateRoleBonus() const {
    return getBaseRate() * specializationBonusRate_;
}
