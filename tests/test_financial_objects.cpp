#include <gtest/gtest.h>

#include "core/date.hpp"
#include "exceptions/invalid_contract_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "financial_objects/probation_period.hpp"
#include "financial_objects/salary.hpp"

#include "test_helpers.hpp"

namespace {
constexpr double kTolerance = 0.001;
constexpr double kBaseRate = 100000.0;
}  // namespace

TEST(DateUtils, DaysBetween) {
    const Date first{std::chrono::year{2026}, std::chrono::October, std::chrono::day{1}};
    const Date second{std::chrono::year{2026}, std::chrono::September, std::chrono::day{1}};
    EXPECT_EQ(daysBetween(second, first), 30);
    EXPECT_EQ(daysBetween(first, second), -30);
    EXPECT_EQ(daysBetween(first, first), 0);
    EXPECT_TRUE(isEarlier(second, first));
    EXPECT_FALSE(isEarlier(first, second));
    EXPECT_TRUE(isSameOrEarlier(first, first));
}

TEST(ProbationPeriodTest, DiscountReducesRateByFifteenPercent) {
    const ProbationPeriod probation{today()};
    EXPECT_DOUBLE_EQ(probation.calculateRate(kBaseRate), kBaseRate * 0.85);
    EXPECT_DOUBLE_EQ(probation.getDiscountRate(), 0.15);
    EXPECT_EQ(probation.getDurationDays(), 30);
}

TEST(ProbationPeriodTest, ActiveOnlyWithinThirtyDays) {
    const Date hire{std::chrono::year{2026}, std::chrono::January, std::chrono::day{10}};
    const ProbationPeriod probation{hire};

    EXPECT_FALSE(probation.isActiveOn(daysBefore(hire, 1)));
    EXPECT_TRUE(probation.isActiveOn(hire));
    EXPECT_TRUE(probation.isActiveOn(daysAfter(hire, 29)));
    EXPECT_FALSE(probation.isActiveOn(daysAfter(hire, 30)));
    EXPECT_EQ(probation.daysRemaining(hire), 30);
    EXPECT_EQ(probation.daysRemaining(daysAfter(hire, 10)), 20);
    EXPECT_EQ(probation.daysRemaining(daysAfter(hire, 40)), 0);
}

TEST(ProbationPeriodTest, RejectsNonPositiveDuration) {
    EXPECT_THROW((ProbationPeriod{today(), 0}), InvalidInputException);
    EXPECT_THROW((ProbationPeriod{today(), -5}), InvalidInputException);
}

TEST(EmploymentContractTest, CreatedWithProbationByDefault) {
    const auto ownedContract = makeContract(kBaseRate, "Бухгалтер");
    const EmploymentContract& contract = *ownedContract;

    EXPECT_EQ(contract.getPosition(), "Бухгалтер");
    EXPECT_DOUBLE_EQ(contract.getMonthlyRate(), kBaseRate);
    EXPECT_TRUE(contract.isOnProbationOn(today()));
    EXPECT_DOUBLE_EQ(contract.getEffectiveRateOn(today()), kBaseRate * 0.85);
    EXPECT_TRUE(contract.isActiveOn(today()));
    EXPECT_FALSE(contract.isTerminated());
}

TEST(EmploymentContractTest, WithoutProbationRateIsFull) {
    EmploymentContract contract("T-1", "Юрист", today(), kBaseRate, false);

    EXPECT_FALSE(contract.isOnProbationOn(today()));
    EXPECT_DOUBLE_EQ(contract.getEffectiveRateOn(today()), kBaseRate);
}

TEST(EmploymentContractTest, RenewEndsProbationPermanently) {
    const Date hire = daysBefore(today(), 90);
    EmploymentContract contract("T-1", "Юрист", hire, kBaseRate);
    ASSERT_TRUE(contract.isOnProbationOn(daysAfter(hire, 10)));
    EXPECT_DOUBLE_EQ(contract.getEffectiveRateOn(daysAfter(hire, 10)), kBaseRate * 0.85);

    contract.renew(kBaseRate * 1.2, daysAfter(hire, 30));

    // Продление закрывает испытательный срок: нового срока не начинается,
    // и ставка становится полной с самого дня продления.
    EXPECT_FALSE(contract.isOnProbationOn(daysAfter(hire, 40)));
    EXPECT_FALSE(contract.isOnProbationOn(today()));
    EXPECT_DOUBLE_EQ(contract.getEffectiveRateOn(daysAfter(hire, 40)), kBaseRate * 1.2);
    EXPECT_DOUBLE_EQ(contract.getMonthlyRate(), kBaseRate * 1.2);
    EXPECT_EQ(contract.getHireDate(), daysAfter(hire, 30));
}

TEST(EmploymentContractTest, RenewRejectsInvalidTerms) {
    EmploymentContract contract("T-1", "Юрист", daysBefore(today(), 90), kBaseRate);

    EXPECT_THROW(contract.renew(0.0, today()), InvalidInputException);
    EXPECT_THROW(contract.renew(kBaseRate, daysBefore(today(), 200)), InvalidContractException);

    contract.terminate(today());
    EXPECT_THROW(contract.renew(kBaseRate, today()), InvalidContractException)
        << "расторгнутый контракт нельзя продлить";
}

TEST(EmploymentContractTest, TerminateDeactivatesContract) {
    EmploymentContract contract("T-1", "Юрист", daysBefore(today(), 10), kBaseRate);
    contract.terminate(today());

    EXPECT_TRUE(contract.isTerminated());
    EXPECT_FALSE(contract.isActiveOn(today()));
    EXPECT_THROW(contract.terminate(daysBefore(today(), 20)), InvalidContractException);
}

TEST(EmploymentContractTest, RejectsInvalidTerms) {
    EXPECT_THROW(*makeContract(0.0), InvalidInputException);
    EXPECT_THROW(*makeContract(-100.0), InvalidInputException);
}

TEST(AdvancePaymentTest, DeductionCappedByRemainingAndAvailable) {
    AdvancePayment advance(15000.0, today());
    EXPECT_DOUBLE_EQ(advance.getRemaining(), 15000.0);
    EXPECT_FALSE(advance.isFullyRepaid());

    EXPECT_DOUBLE_EQ(advance.applyDeduction(10000.0), 10000.0);
    EXPECT_DOUBLE_EQ(advance.getRemaining(), 5000.0);
    EXPECT_FALSE(advance.isFullyRepaid());

    // остаток больше не списывается, чем есть на руках
    EXPECT_DOUBLE_EQ(advance.applyDeduction(3000.0), 3000.0);
    EXPECT_DOUBLE_EQ(advance.getRemaining(), 2000.0);

    // остаток меньше доступного — списывается ровно остаток, долга не возникает
    EXPECT_DOUBLE_EQ(advance.applyDeduction(3000.0), 2000.0);
    EXPECT_TRUE(advance.isFullyRepaid());
    EXPECT_DOUBLE_EQ(advance.getRemaining(), 0.0);

    // остаток кончился — дальше не списывается, долга не возникает
    EXPECT_DOUBLE_EQ(advance.applyDeduction(50000.0), 0.0);
    EXPECT_TRUE(advance.isFullyRepaid());
}

TEST(AdvancePaymentTest, RejectsNonPositiveAmount) {
    EXPECT_THROW(AdvancePayment(0.0, today()), InvalidInputException);
    EXPECT_THROW(AdvancePayment(-1.0, today()), InvalidInputException);
}

TEST(SalaryTest, BonusAffectsGrossAndNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyBonus(10000.0);

    EXPECT_DOUBLE_EQ(salary.getGross(), 110000.0);
    EXPECT_DOUBLE_EQ(salary.getNet(), 110000.0);
}

TEST(SalaryTest, DeductionReducesGrossAndNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyDeduction(20000.0);

    EXPECT_NEAR(salary.getGross(), 80000.0, kTolerance);
    EXPECT_NEAR(salary.getNet(), 80000.0, kTolerance);
}

TEST(SalaryTest, RepaymentReducesOnlyNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyRepayment(15000.0);

    EXPECT_NEAR(salary.getGross(), 100000.0, kTolerance);
    EXPECT_NEAR(salary.getNet(), 85000.0, kTolerance);
}

TEST(SalaryTest, TaxReducesOnlyNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyTax(13000.0);

    EXPECT_NEAR(salary.getGross(), 100000.0, kTolerance);
    EXPECT_NEAR(salary.getTaxDeduction(), 13000.0, kTolerance);
    EXPECT_NEAR(salary.getNet(), 87000.0, kTolerance);
}

TEST(SalaryTest, RejectsInvalidAmounts) {
    Salary salary(1000.0, 0.0, 1000.0);

    EXPECT_THROW(salary.applyBonus(-1.0), InvalidInputException);
    EXPECT_THROW(salary.applyDeduction(-1.0), InvalidInputException);
    EXPECT_THROW(salary.applyRepayment(-1.0), InvalidInputException);
    EXPECT_THROW(salary.applyTax(-1.0), InvalidInputException);
    EXPECT_THROW(salary.applyTax(2000.0), InvalidInputException);
}
