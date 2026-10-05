#pragma once

#include "core/date.hpp"

class AdvancePayment {
public:
    AdvancePayment() = default;
    AdvancePayment(double amount, Date issueDate);

    double getAmount() const;
    double getRepaidAmount() const;
    double getRemaining() const;
    bool isFullyRepaid() const;
    Date getIssueDate() const;

    double applyDeduction(double availableAmount);

private:
    double amount_ = 0.0;
    double repaidAmount_ = 0.0;
    Date issueDate_;
};
