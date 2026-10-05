#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class Accountant : public Specialist {
public:
    static constexpr double kBaseSpecialistRate = 0.10;
    static constexpr double kSeniorCertifiedBonus = 0.15;

    Accountant(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
               std::unique_ptr<AdvancePayment> advance, std::string certificationLevel);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getCertificationLevel() const;
    bool isSeniorCertified() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string certificationLevel_;
};
