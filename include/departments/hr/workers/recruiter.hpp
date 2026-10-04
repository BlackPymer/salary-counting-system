#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

// Рекрутер: подбирает персонал, его результат — закрытые вакансии.
// Также готовит условия найма: контракт с испытательным сроком и аванс.
class Recruiter : public Specialist {
public:
    // Аванс не может превышать эту долю месячной ставки.
    static constexpr double kMaxAdvanceRate = 0.15;

    Recruiter(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
              std::unique_ptr<AdvancePayment> advance);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    int getVacanciesClosed() const;
    int getCandidatesInProcess() const;
    void registerCandidate();
    void closeVacancy();

    double getClosingRate() const;

    // Проверка условий до найма: ставка положительна, аванс в пределах нормы.
    void validateTerms(double monthlyRate, double advanceAmount) const;

    std::unique_ptr<EmploymentContract> createContract(int workerId, const std::string& position,
                                                       double monthlyRate) const;
    std::unique_ptr<AdvancePayment> createAdvance(double amount) const;

    // Единственный источник формата номера контракта.
    static std::string contractNumberFor(int workerId);

protected:
    double calculateRoleBonus() const override;

private:
    int vacanciesClosed_ = 0;
    int candidatesInProcess_ = 0;
};
