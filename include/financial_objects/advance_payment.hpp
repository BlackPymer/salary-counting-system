#pragma once

#include "core/date.hpp"

class AdvancePayment {
public:
    static constexpr int kRepaymentTermDays = 30;

    AdvancePayment() = default;
    AdvancePayment(double amount, Date issueDate);

    double getAmount() const;
    double getRepaidAmount() const;
    double getRemaining() const;
    bool isFullyRepaid() const;
    Date getIssueDate() const;
    Date getRepaymentDue() const;

    double applyDeduction(double availableAmount);

private:
    double amount_ = 0.0;
    double repaidAmount_ = 0.0;
    Date issueDate_;
    Date repaymentDue_;
};
