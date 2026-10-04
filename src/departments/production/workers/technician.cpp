#include "departments/production/workers/technician.hpp"

#include <algorithm>

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kBaseTechnicianRate = 0.10;
constexpr double kCertificationBonusPerCert = 0.04;
constexpr double kOutputBonus = 0.06;
constexpr int kOutputThreshold = 100;
constexpr std::size_t kMultiCertifiedMinimum = 2;
}  // namespace

Technician::Technician(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                       std::unique_ptr<AdvancePayment> advance,
                       std::vector<std::string> equipmentCertifications)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      equipmentCertifications_(std::move(equipmentCertifications)) {
    if (equipmentCertifications_.empty()) {
        throw InvalidInputException("Техник должен иметь хотя бы одну сертификацию");
    }
}

std::string Technician::getRole() const {
    return "Technician";
}

std::string Technician::getSpecialization() const {
    return "Обслуживание производственного оборудования";
}

const std::vector<std::string>& Technician::getEquipmentCertifications() const {
    return equipmentCertifications_;
}

int Technician::getUnitsProduced() const {
    return unitsProduced_;
}

void Technician::produceUnits(int count) {
    if (count < 0) {
        throw InvalidInputException("Количество единиц не может быть отрицательным");
    }
    unitsProduced_ += count;
}

bool Technician::isCertifiedOn(const std::string& equipment) const {
    return std::find(equipmentCertifications_.begin(), equipmentCertifications_.end(), equipment) !=
           equipmentCertifications_.end();
}

bool Technician::isMultiCertified() const {
    return equipmentCertifications_.size() >= kMultiCertifiedMinimum;
}

double Technician::calculateRoleBonus() const {
    const double certificationBonus =
        static_cast<double>(equipmentCertifications_.size()) * kCertificationBonusPerCert;
    const double outputBonus =
        unitsProduced_ >= kOutputThreshold && isMultiCertified() ? kOutputBonus : 0.0;
    return getBaseRate() * (kBaseTechnicianRate + certificationBonus + outputBonus);
}
