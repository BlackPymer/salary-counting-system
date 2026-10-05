#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class SecurityGuard : public Specialist {
public:
    SecurityGuard(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                  std::unique_ptr<AdvancePayment> advance, std::string shiftType);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getShiftType() const;
    int getShiftsCompleted() const;
    int getIncidentsPrevented() const;
    void completeShift();
    void preventIncident();

    bool isNightShift() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string shiftType_;
    int shiftsCompleted_ = 0;
    int incidentsPrevented_ = 0;
};
