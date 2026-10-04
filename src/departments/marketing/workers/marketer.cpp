#include "departments/marketing/workers/marketer.hpp"

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kBaseMarketerRate = 0.10;
constexpr double kEfficiencyBonus = 0.08;
constexpr double kTargetLeadsPerCampaign = 50.0;
}  // namespace

Marketer::Marketer(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                   std::unique_ptr<AdvancePayment> advance)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)) {}

std::string Marketer::getRole() const {
    return "Marketer";
}

std::string Marketer::getSpecialization() const {
    return "Продвижение и реклама";
}

int Marketer::getCampaignsLaunched() const {
    return campaignsLaunched_;
}

int Marketer::getLeadsGenerated() const {
    return leadsGenerated_;
}

void Marketer::launchCampaign(int leadsGenerated) {
    if (leadsGenerated < 0) {
        throw InvalidInputException("Число лидов не может быть отрицательным");
    }
    ++campaignsLaunched_;
    leadsGenerated_ += leadsGenerated;
}

double Marketer::getLeadsPerCampaign() const {
    if (campaignsLaunched_ == 0) {
        return 0.0;
    }
    return static_cast<double>(leadsGenerated_) / campaignsLaunched_;
}

double Marketer::calculateRoleBonus() const {
    return getBaseRate() *
           (kBaseMarketerRate +
            (getLeadsPerCampaign() >= kTargetLeadsPerCampaign ? kEfficiencyBonus : 0.0));
}
