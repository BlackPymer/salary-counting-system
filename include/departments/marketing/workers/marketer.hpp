#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class Marketer : public Specialist {
public:
    static constexpr double kBaseMarketerRate = 0.10;
    static constexpr double kEfficiencyBonus = 0.08;
    static constexpr double kTargetLeadsPerCampaign = 50.0;

    Marketer(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
             std::unique_ptr<AdvancePayment> advance);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    int getCampaignsLaunched() const;
    int getLeadsGenerated() const;
    void launchCampaign(int leadsGenerated);

    double getLeadsPerCampaign() const;

protected:
    double calculateRoleBonus() const override;

private:
    int campaignsLaunched_ = 0;
    int leadsGenerated_ = 0;
};
