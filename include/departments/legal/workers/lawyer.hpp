#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class Lawyer : public Specialist {
public:
    static constexpr double kBaseLawyerRate = 0.13;
    static constexpr double kCaseloadBonus = 0.07;
    static constexpr int kCaseloadThreshold = 5;

    Lawyer(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
           std::unique_ptr<AdvancePayment> advance, std::string barNumber);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getBarNumber() const;
    int getCasesHandled() const;
    void handleCase();

    bool reviewsEmploymentContracts() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string barNumber_;
    int casesHandled_ = 0;
};
