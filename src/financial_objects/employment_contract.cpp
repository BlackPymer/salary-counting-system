#include "financial_objects/employment_contract.hpp"

#include "exceptions/invalid_contract_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"

EmploymentContract::EmploymentContract(std::string contractNumber, std::string position,
                                       Date hireDate, double monthlyRate, bool withProbation)
    : contractNumber_(std::move(contractNumber)),
      position_(std::move(position)),
      hireDate_(hireDate),
      monthlyRate_(monthlyRate) {
    if (monthlyRate <= 0.0) {
        throw InvalidInputException("Месячная ставка в контракте должна быть положительной");
    }
    if (withProbation) {
        probationPeriod_ = std::make_unique<ProbationPeriod>(hireDate);
    }
}

bool EmploymentContract::isActiveOn(const Date& date) const {
    if (terminated_) {
        return false;
    }
    return !isEarlier(date, hireDate_);
}

bool EmploymentContract::isTerminated() const {
    return terminated_;
}

bool EmploymentContract::isOnProbationOn(const Date& date) const {
    return probationPeriod_ != nullptr && probationPeriod_->isActiveOn(date) && isActiveOn(date);
}

void EmploymentContract::renew(double newMonthlyRate, Date renewalDate) {
    if (newMonthlyRate <= 0.0) {
        throw InvalidInputException("Ставка при продлении должна быть положительной");
    }
    if (terminated_) {
        throw InvalidContractException("Нельзя продлить расторгнутый контракт");
    }
    if (isEarlier(renewalDate, hireDate_)) {
        throw InvalidContractException("Дата продления раньше даты найма");
    }
    monthlyRate_ = newMonthlyRate;
    probationPeriod_.reset();
    hireDate_ = renewalDate;
    ++renewalCount_;
}

void EmploymentContract::terminate(Date terminationDate) {
    if (isEarlier(terminationDate, hireDate_)) {
        throw InvalidContractException("Дата расторжения раньше даты найма");
    }
    terminated_ = true;
    terminationDate_ = terminationDate;
}

double EmploymentContract::getMonthlyRate() const {
    return monthlyRate_;
}

double EmploymentContract::getEffectiveRateOn(const Date& date) const {
    if (probationPeriod_ != nullptr && probationPeriod_->isActiveOn(date)) {
        return probationPeriod_->calculateRate(monthlyRate_);
    }
    return monthlyRate_;
}

const std::string& EmploymentContract::getContractNumber() const {
    return contractNumber_;
}

const std::string& EmploymentContract::getPosition() const {
    return position_;
}

Date EmploymentContract::getHireDate() const {
    return hireDate_;
}

Date EmploymentContract::getTerminationDate() const {
    return terminationDate_;
}

const ProbationPeriod& EmploymentContract::getProbationPeriod() const {
    return *probationPeriod_;
}

int EmploymentContract::getRenewalCount() const {
    return renewalCount_;
}
