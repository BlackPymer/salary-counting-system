#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

// Юрист: надбавка за количество завершённых дел.
class Lawyer : public Specialist {
public:
    Lawyer(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
           std::unique_ptr<AdvancePayment> advance, std::string barNumber);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getBarNumber() const;
    int getCasesHandled() const;
    void handleCase();

    // Работает ли юрист с контрактами сотрудников.
    bool reviewsEmploymentContracts() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string barNumber_;
    int casesHandled_ = 0;
};
