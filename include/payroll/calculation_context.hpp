#pragma once

#include "core/date.hpp"
#include "financial_objects/payroll_period.hpp"
#include "workers/worker.hpp"

class CalculationContext {
public:
    CalculationContext(const Worker* worker, const PayrollPeriod& period);
    CalculationContext(const Worker* worker, const PayrollPeriod& period,
                       const Date& calculationDate);

    const Worker& getWorker() const;
    const PayrollPeriod& getPeriod() const;
    const Date& getCalculationDate() const;

    double getBaseRate() const;
    double getEffectiveRate() const;
    bool isOnProbation() const;
    bool isActive() const;

    double getOvertimeHours() const;
    int getTotalAbsentDays() const;
    double getAveragePayRate() const;
    const std::vector<std::unique_ptr<Absence>>& getAbsences() const;

    const AdvancePayment& getAdvance() const;
    const EmploymentContract& getContract() const;

private:
    const Worker* worker_;
    PayrollPeriod period_;
    Date calculationDate_;
};
