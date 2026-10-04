#include "departments/customer_support/workers/customer_support_agent.hpp"

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kBaseSupportRate = 0.08;
constexpr double kQualityBonus = 0.07;
constexpr int kServiceStandardMinutes = 15;
}  // namespace

CustomerSupportAgent::CustomerSupportAgent(int id, std::string fullName,
                                           std::unique_ptr<EmploymentContract> contract,
                                           std::unique_ptr<AdvancePayment> advance,
                                           std::string supportChannel)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      supportChannel_(std::move(supportChannel)) {
    if (supportChannel_.empty()) {
        throw InvalidInputException("Канал поддержки не может быть пустым");
    }
}

std::string CustomerSupportAgent::getRole() const {
    return "CustomerSupportAgent";
}

std::string CustomerSupportAgent::getSpecialization() const {
    return "Обработка обращений клиентов (" + supportChannel_ + ")";
}

const std::string& CustomerSupportAgent::getSupportChannel() const {
    return supportChannel_;
}

int CustomerSupportAgent::getTicketsResolved() const {
    return ticketsResolved_;
}

int CustomerSupportAgent::getAverageResponseMinutes() const {
    if (ticketsResolved_ == 0) {
        return 0;
    }
    return totalResponseMinutes_ / ticketsResolved_;
}

void CustomerSupportAgent::resolveTicket(int responseMinutes) {
    if (responseMinutes < 0) {
        throw InvalidInputException("Время ответа не может быть отрицательным");
    }
    ++ticketsResolved_;
    totalResponseMinutes_ += responseMinutes;
}

bool CustomerSupportAgent::exceedsServiceStandard() const {
    const int average = getAverageResponseMinutes();
    return average > 0 && average <= kServiceStandardMinutes;
}

double CustomerSupportAgent::calculateRoleBonus() const {
    return getBaseRate() * (kBaseSupportRate + (exceedsServiceStandard() ? kQualityBonus : 0.0));
}
