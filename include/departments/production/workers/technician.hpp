#pragma once

#include <string>
#include <vector>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class Technician : public Specialist {
public:
    Technician(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
               std::unique_ptr<AdvancePayment> advance,
               std::vector<std::string> equipmentCertifications);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::vector<std::string>& getEquipmentCertifications() const;
    int getUnitsProduced() const;
    void produceUnits(int count);

    bool isCertifiedOn(const std::string& equipment) const;
    bool isMultiCertified() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::vector<std::string> equipmentCertifications_;
    int unitsProduced_ = 0;
};
