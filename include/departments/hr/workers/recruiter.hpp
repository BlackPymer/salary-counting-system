#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

// Рекрутер: подбирает персонал, его результат — закрытые вакансии.
class Recruiter : public Specialist {
public:
    Recruiter(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
              std::unique_ptr<AdvancePayment> advance);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    int getVacanciesClosed() const;
    int getCandidatesInProcess() const;
    void registerCandidate();
    void closeVacancy();

    double getClosingRate() const;

protected:
    double calculateRoleBonus() const override;

private:
    int vacanciesClosed_ = 0;
    int candidatesInProcess_ = 0;
};
