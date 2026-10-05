#include "departments/accounting/workers/accountant.hpp"

Accountant::Accountant(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                       std::unique_ptr<AdvancePayment> advance, std::string certificationLevel)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      certificationLevel_(std::move(certificationLevel)) {}

std::string Accountant::getRole() const {
    return "Accountant";
}

std::string Accountant::getSpecialization() const {
    return "Бухгалтерский и налоговый учёт";
}

const std::string& Accountant::getCertificationLevel() const {
    return certificationLevel_;
}

bool Accountant::isSeniorCertified() const {
    return certificationLevel_ == "senior" || certificationLevel_ == "chief";
}

double Accountant::calculateRoleBonus() const {
    return getBaseRate() *
           (kBaseSpecialistRate + (isSeniorCertified() ? kSeniorCertifiedBonus : 0.0));
}
