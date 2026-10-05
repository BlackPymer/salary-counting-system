#pragma once

#include <memory>
#include <string>

#include "core/date.hpp"
#include "probation_period.hpp"

class EmploymentContract {
public:
    EmploymentContract() = default;
    EmploymentContract(std::string contractNumber, std::string position, Date hireDate,
                       double monthlyRate, bool withProbation = true);
    ~EmploymentContract() = default;

    EmploymentContract(const EmploymentContract&) = delete;
    EmploymentContract& operator=(const EmploymentContract&) = delete;
    EmploymentContract(EmploymentContract&&) = default;
    EmploymentContract& operator=(EmploymentContract&&) = default;

    bool isActiveOn(const Date& date) const;
    bool isTerminated() const;
    bool isOnProbationOn(const Date& date) const;

    void renew(double newMonthlyRate, Date renewalDate);
    void terminate(Date terminationDate);

    double getMonthlyRate() const;

    double getEffectiveRateOn(const Date& date) const;

    const std::string& getContractNumber() const;
    const std::string& getPosition() const;
    Date getHireDate() const;
    Date getTerminationDate() const;
    const ProbationPeriod& getProbationPeriod() const;
    int getRenewalCount() const;

private:
    std::string contractNumber_;
    std::string position_;
    Date hireDate_;
    double monthlyRate_ = 0.0;
    bool terminated_ = false;
    int renewalCount_ = 0;
    Date terminationDate_;
    std::unique_ptr<ProbationPeriod> probationPeriod_;
};
