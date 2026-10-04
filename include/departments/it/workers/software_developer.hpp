#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

// Разработчик ПО: надбавка за закрытые задачи.
class SoftwareDeveloper : public Specialist {
public:
    SoftwareDeveloper(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                      std::unique_ptr<AdvancePayment> advance, std::string primaryLanguage);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getPrimaryLanguage() const;
    int getTasksCompleted() const;
    int getCodeReviewsGiven() const;
    void completeTask();
    void reviewCode();

    // Одновременно закрывает задачи и проводит ревью — признак сильного сотрудника.
    bool isFullCycle() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string primaryLanguage_;
    int tasksCompleted_ = 0;
    int codeReviewsGiven_ = 0;
};
