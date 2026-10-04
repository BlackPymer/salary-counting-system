#include "departments/hr/workers/recruiter.hpp"

namespace {
constexpr double kBaseRecruiterRate = 0.10;
constexpr double kHighClosingRateBonus = 0.10;
constexpr double kTargetClosingRate = 0.5;
}  // namespace

Recruiter::Recruiter(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                     std::unique_ptr<AdvancePayment> advance)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)) {}

std::string Recruiter::getRole() const {
    return "Recruiter";
}

std::string Recruiter::getSpecialization() const {
    return "Подбор и адаптация персонала";
}

int Recruiter::getVacanciesClosed() const {
    return vacanciesClosed_;
}

int Recruiter::getCandidatesInProcess() const {
    return candidatesInProcess_;
}

void Recruiter::registerCandidate() {
    ++candidatesInProcess_;
}

void Recruiter::closeVacancy() {
    ++vacanciesClosed_;
    if (candidatesInProcess_ > 0) {
        --candidatesInProcess_;
    }
}

double Recruiter::getClosingRate() const {
    const int total = vacanciesClosed_ + candidatesInProcess_;
    if (total == 0) {
        return 0.0;
    }
    return static_cast<double>(vacanciesClosed_) / total;
}

double Recruiter::calculateRoleBonus() const {
    return getBaseRate() * (kBaseRecruiterRate + (getClosingRate() >= kTargetClosingRate
                                                      ? kHighClosingRateBonus
                                                      : 0.0));
}
