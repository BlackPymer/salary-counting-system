#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

// Оператор клиентской поддержки: надбавка зависит от скорости ответа.
class CustomerSupportAgent : public Specialist {
public:
    CustomerSupportAgent(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                         std::unique_ptr<AdvancePayment> advance, std::string supportChannel);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    const std::string& getSupportChannel() const;
    int getTicketsResolved() const;
    int getAverageResponseMinutes() const;
    void resolveTicket(int responseMinutes);

    // Быстрее нормативного ответа — доплата за качество.
    bool exceedsServiceStandard() const;

protected:
    double calculateRoleBonus() const override;

private:
    std::string supportChannel_;
    int ticketsResolved_ = 0;
    int totalResponseMinutes_ = 0;
};
