#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class ResearchScientist : public Specialist {
public:
    ResearchScientist(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                      std::unique_ptr<AdvancePayment> advance, std::string researchArea);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getResearchArea() const;
    int getProjectsCompleted() const;
    int getPublicationsCount() const;
    void completeProject();
    void publishPaper();

    bool hasPublicationRecord() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string researchArea_;
    int projectsCompleted_ = 0;
    int publicationsCount_ = 0;
};
