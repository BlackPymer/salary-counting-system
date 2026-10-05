#include <UnitTest++/UnitTest++.h>

#include "core/date.hpp"
#include "exceptions/invalid_contract_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "financial_objects/payroll_period.hpp"
#include "financial_objects/probation_period.hpp"
#include "financial_objects/salary.hpp"

#include "test_helpers.hpp"

namespace {
constexpr double kTolerance = 0.001;
constexpr double kBaseRate = 100000.0;
}  // namespace

TEST(DateUtils_DaysBetween) {
    const Date first{std::chrono::year{2026}, std::chrono::October, std::chrono::day{1}};
    const Date second{std::chrono::year{2026}, std::chrono::September, std::chrono::day{1}};
    CHECK_EQUAL(30, daysBetween(second, first));
    CHECK_EQUAL(-30, daysBetween(first, second));
    CHECK_EQUAL(0, daysBetween(first, first));
    CHECK(isEarlier(second, first));
    CHECK(!isEarlier(first, second));
    CHECK(isSameOrEarlier(first, first));
}

TEST(ProbationPeriodTest_DiscountReducesRateByFifteenPercent) {
    const ProbationPeriod probation{today()};
    CHECK_CLOSE(kBaseRate * 0.85, probation.calculateRate(kBaseRate), kTolerance);
    CHECK_CLOSE(0.15, probation.getDiscountRate(), kTolerance);
    CHECK_EQUAL(30, probation.getDurationDays());
}

TEST(ProbationPeriodTest_ActiveOnlyWithinThirtyDays) {
    const Date hire{std::chrono::year{2026}, std::chrono::January, std::chrono::day{10}};
    const ProbationPeriod probation{hire};

    CHECK(!probation.isActiveOn(daysBefore(hire, 1)));
    CHECK(probation.isActiveOn(hire));
    CHECK(probation.isActiveOn(daysAfter(hire, 29)));
    CHECK(!probation.isActiveOn(daysAfter(hire, 30)));
    CHECK_EQUAL(30, probation.daysRemaining(hire));
    CHECK_EQUAL(20, probation.daysRemaining(daysAfter(hire, 10)));
    CHECK_EQUAL(0, probation.daysRemaining(daysAfter(hire, 40)));
}

TEST(ProbationPeriodTest_RejectsNonPositiveDuration) {
    CHECK_THROW((ProbationPeriod{today(), 0}), InvalidInputException);
    CHECK_THROW((ProbationPeriod{today(), -5}), InvalidInputException);
}

TEST(EmploymentContractTest_CreatedWithProbationByDefault) {
    const auto ownedContract = makeContract(kBaseRate, "Бухгалтер");
    const EmploymentContract& contract = *ownedContract;

    CHECK_EQUAL("Бухгалтер", contract.getPosition());
    CHECK_CLOSE(kBaseRate, contract.getMonthlyRate(), kTolerance);
    CHECK(contract.isOnProbationOn(today()));
    CHECK_CLOSE(kBaseRate * 0.85, contract.getEffectiveRateOn(today()), kTolerance);
    CHECK(contract.isActiveOn(today()));
    CHECK(!contract.isTerminated());
}

TEST(EmploymentContractTest_WithoutProbationRateIsFull) {
    EmploymentContract contract("T-1", "Юрист", today(), kBaseRate, false);

    CHECK(!contract.isOnProbationOn(today()));
    CHECK_CLOSE(kBaseRate, contract.getEffectiveRateOn(today()), kTolerance);
}

TEST(EmploymentContractTest_RenewEndsProbationPermanently) {
    const Date hire = daysBefore(today(), 90);
    EmploymentContract contract("T-1", "Юрист", hire, kBaseRate);
    CHECK(contract.isOnProbationOn(daysAfter(hire, 10)));
    CHECK_CLOSE(kBaseRate * 0.85, contract.getEffectiveRateOn(daysAfter(hire, 10)), kTolerance);

    contract.renew(kBaseRate * 1.2, daysAfter(hire, 30));

    CHECK(!contract.isOnProbationOn(daysAfter(hire, 40)));
    CHECK(!contract.isOnProbationOn(today()));
    CHECK_CLOSE(kBaseRate * 1.2, contract.getEffectiveRateOn(daysAfter(hire, 40)), kTolerance);
    CHECK_CLOSE(kBaseRate * 1.2, contract.getMonthlyRate(), kTolerance);
    CHECK_EQUAL(daysAfter(hire, 30), contract.getHireDate());
}

TEST(EmploymentContractTest_RenewRejectsInvalidTerms) {
    EmploymentContract contract("T-1", "Юрист", daysBefore(today(), 90), kBaseRate);

    CHECK_THROW(contract.renew(0.0, today()), InvalidInputException);
    CHECK_THROW(contract.renew(kBaseRate, daysBefore(today(), 200)), InvalidContractException);

    contract.terminate(today());
    CHECK_THROW(contract.renew(kBaseRate, today()), InvalidContractException);
}

TEST(EmploymentContractTest_TerminateDeactivatesContract) {
    EmploymentContract contract("T-1", "Юрист", daysBefore(today(), 10), kBaseRate);
    contract.terminate(today());

    CHECK(contract.isTerminated());
    CHECK(!contract.isActiveOn(today()));
    CHECK_THROW(contract.terminate(daysBefore(today(), 20)), InvalidContractException);
}

TEST(EmploymentContractTest_RejectsInvalidTerms) {
    CHECK_THROW(*makeContract(0.0), InvalidInputException);
    CHECK_THROW(*makeContract(-100.0), InvalidInputException);
}

TEST(AdvancePaymentTest_DeductionCappedByRemainingAndAvailable) {
    AdvancePayment advance(15000.0, today());
    CHECK_CLOSE(15000.0, advance.getRemaining(), kTolerance);
    CHECK(!advance.isFullyRepaid());

    CHECK_CLOSE(10000.0, advance.applyDeduction(10000.0), kTolerance);
    CHECK_CLOSE(5000.0, advance.getRemaining(), kTolerance);
    CHECK(!advance.isFullyRepaid());

    CHECK_CLOSE(3000.0, advance.applyDeduction(3000.0), kTolerance);
    CHECK_CLOSE(2000.0, advance.getRemaining(), kTolerance);

    CHECK_CLOSE(2000.0, advance.applyDeduction(3000.0), kTolerance);
    CHECK(advance.isFullyRepaid());
    CHECK_CLOSE(0.0, advance.getRemaining(), kTolerance);

    CHECK_CLOSE(0.0, advance.applyDeduction(50000.0), kTolerance);
    CHECK(advance.isFullyRepaid());
}

TEST(AdvancePaymentTest_RejectsNonPositiveAmount) {
    CHECK_THROW(AdvancePayment(0.0, today()), InvalidInputException);
    CHECK_THROW(AdvancePayment(-1.0, today()), InvalidInputException);
}

TEST(SalaryTest_BonusAffectsGrossAndNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyBonus(10000.0);

    CHECK_CLOSE(110000.0, salary.getGross(), kTolerance);
    CHECK_CLOSE(110000.0, salary.getNet(), kTolerance);
}

TEST(SalaryTest_DeductionReducesGrossAndNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyDeduction(20000.0);

    CHECK_CLOSE(80000.0, salary.getGross(), kTolerance);
    CHECK_CLOSE(80000.0, salary.getNet(), kTolerance);
}

TEST(SalaryTest_RepaymentReducesOnlyNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyRepayment(15000.0);

    CHECK_CLOSE(100000.0, salary.getGross(), kTolerance);
    CHECK_CLOSE(85000.0, salary.getNet(), kTolerance);
}

TEST(SalaryTest_TaxReducesOnlyNet) {
    Salary salary(100000.0, 0.0, 100000.0);
    salary.applyTax(13000.0);

    CHECK_CLOSE(100000.0, salary.getGross(), kTolerance);
    CHECK_CLOSE(13000.0, salary.getTaxDeduction(), kTolerance);
    CHECK_CLOSE(87000.0, salary.getNet(), kTolerance);
}

TEST(SalaryTest_RejectsInvalidAmounts) {
    Salary salary(1000.0, 0.0, 1000.0);

    CHECK_THROW(salary.applyBonus(-1.0), InvalidInputException);
    CHECK_THROW(salary.applyDeduction(-1.0), InvalidInputException);
    CHECK_THROW(salary.applyRepayment(-1.0), InvalidInputException);
    CHECK_THROW(salary.applyTax(-1.0), InvalidInputException);
    CHECK_THROW(salary.applyTax(2000.0), InvalidInputException);
}

TEST(PayrollPeriodTest_BasicProperties) {
    PayrollPeriod period(2026, 10);
    CHECK_EQUAL(2026, period.getYear());
    CHECK_EQUAL(10, period.getMonth());
    CHECK_EQUAL("2026-10", period.toString());
    CHECK(period.contains(period.getFirstDay()));
    CHECK(period.contains(period.getLastDay()));
}

TEST(PayrollPeriodTest_ContainsOtherDates) {
    PayrollPeriod period(2026, 2);
    const Date first{std::chrono::year{2026}, std::chrono::February, std::chrono::day{1}};
    const Date last{std::chrono::year{2026}, std::chrono::February, std::chrono::day{28}};
    const Date next{std::chrono::year{2026}, std::chrono::March, std::chrono::day{1}};
    const Date prev{std::chrono::year{2026}, std::chrono::January, std::chrono::day{31}};
    CHECK(period.contains(first));
    CHECK(period.contains(last));
    CHECK(!period.contains(next));
    CHECK(!period.contains(prev));
}

TEST(PayrollPeriodTest_Validation) {
    CHECK_THROW(PayrollPeriod(2026, 0), InvalidInputException);
    CHECK_THROW(PayrollPeriod(2026, 13), InvalidInputException);
    CHECK_THROW(PayrollPeriod(0, 1), InvalidInputException);
    CHECK_THROW(PayrollPeriod(-1, 5), InvalidInputException);
}
