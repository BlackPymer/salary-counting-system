#include "departments/hr/workers/recruiter.hpp"

#include "core/date.hpp"
#include "exceptions/invalid_input_exception.hpp"

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
    return getBaseRate() * (kBaseRecruiterRate +
                            (getClosingRate() >= kTargetClosingRate ? kHighClosingRateBonus : 0.0));
}

void Recruiter::validateTerms(double monthlyRate, double advanceAmount) const {
    if (monthlyRate <= 0.0) {
        throw InvalidInputException("Месячная ставка должна быть положительной");
    }
    if (advanceAmount <= 0.0) {
        throw InvalidInputException("Аванс должен быть выплачен при найме");
    }
    if (advanceAmount > monthlyRate * kMaxAdvanceRate) {
        throw InvalidInputException("Аванс превышает " +
                                    std::to_string(static_cast<int>(kMaxAdvanceRate * 100)) +
                                    "% месячной ставки");
    }
}

std::unique_ptr<EmploymentContract> Recruiter::createContract(int workerId,
                                                              const std::string& position,
                                                              double monthlyRate) const {
    validateTerms(monthlyRate, monthlyRate * kMaxAdvanceRate);
    return std::make_unique<EmploymentContract>(contractNumberFor(workerId), position, today(),
                                                monthlyRate);
}

std::string Recruiter::contractNumberFor(int workerId) {
    return "T-" + std::to_string(workerId);
}

std::unique_ptr<AdvancePayment> Recruiter::createAdvance(double amount) const {
    if (amount <= 0.0) {
        throw InvalidInputException("Сумма аванса должна быть положительной");
    }
    return std::make_unique<AdvancePayment>(amount, today());
}
