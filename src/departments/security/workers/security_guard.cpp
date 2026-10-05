#include "departments/security/workers/security_guard.hpp"

#include "exceptions/invalid_input_exception.hpp"

SecurityGuard::SecurityGuard(int id, std::string fullName,
                             std::unique_ptr<EmploymentContract> contract,
                             std::unique_ptr<AdvancePayment> advance, std::string shiftType)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      shiftType_(std::move(shiftType)) {
    if (shiftType_.empty()) {
        throw InvalidInputException("Тип смены обязателен");
    }
}

std::string SecurityGuard::getRole() const {
    return "SecurityGuard";
}

std::string SecurityGuard::getSpecialization() const {
    return "Охрана объектов, смена " + shiftType_;
}

const std::string& SecurityGuard::getShiftType() const {
    return shiftType_;
}

int SecurityGuard::getShiftsCompleted() const {
    return shiftsCompleted_;
}

int SecurityGuard::getIncidentsPrevented() const {
    return incidentsPrevented_;
}

void SecurityGuard::completeShift() {
    ++shiftsCompleted_;
}

void SecurityGuard::preventIncident() {
    ++incidentsPrevented_;
}

bool SecurityGuard::isNightShift() const {
    return shiftType_ == kNightShift;
}

double SecurityGuard::calculateRoleBonus() const {
    const double shiftBonus = isNightShift() ? kNightShiftBonus : 0.0;
    const double incidentBonus =
        incidentsPrevented_ >= kIncidentBonusThreshold ? kIncidentBonus : 0.0;
    const double seniorityBonus = shiftsCompleted_ >= kShiftThreshold ? kIncidentBonus : 0.0;
    return getBaseRate() * (kBaseGuardRate + shiftBonus + incidentBonus + seniorityBonus);
}
