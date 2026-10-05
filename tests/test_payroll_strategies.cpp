#include <UnitTest++/UnitTest++.h>

#include "core/worker_factory.hpp"
#include "financial_objects/payroll_period.hpp"
#include "payroll/calculation_context.hpp"
#include "payroll/strategies/absence_pay_strategy.hpp"
#include "payroll/strategies/advance_deduction_strategy.hpp"
#include "payroll/strategies/probation_discount_strategy.hpp"
#include "payroll/strategies/tax_strategy.hpp"
#include "test_helpers.hpp"

namespace {
constexpr double kTolerance = 0.001;
}

TEST(TaxStrategyTest_ProgressiveScale) {
    TaxStrategy tax;
    auto contract = makeContract(100000.0, "Dev");
    auto advance = makeAdvance(1000.0);
    auto worker = WorkerFactory::create(1, "Иван", WorkerType::SoftwareDeveloper,
                                        std::move(contract), std::move(advance));
    PayrollPeriod period(2026, 10);
    CalculationContext ctx(worker.get(), period);

    Salary s1(50000.0, 0.0, 50000.0);
    tax.apply(ctx, s1);
    CHECK_CLOSE(50000.0 * 0.13, s1.getTaxDeduction(), kTolerance);
    CHECK_CLOSE(50000.0 - 50000.0 * 0.13, s1.getNet(), kTolerance);

    Salary s2(200000.0, 0.0, 200000.0);
    tax.apply(ctx, s2);
    double tax2 = 200000.0 * 0.13;
    CHECK_CLOSE(tax2, s2.getTaxDeduction(), kTolerance);

    Salary s3(300000.0, 0.0, 300000.0);
    tax.apply(ctx, s3);
    double tax3 = 200000.0 * 0.13 + 100000.0 * 0.15;
    CHECK_CLOSE(tax3, s3.getTaxDeduction(), kTolerance);
}

TEST(AdvanceDeductionStrategyTest_AppliesAdvance) {
    AdvanceDeductionStrategy adv;
    auto contract = makeContract(100000.0, "Dev");
    auto advance = makeAdvance(30000.0);
    auto worker = WorkerFactory::create(1, "Иван", WorkerType::SoftwareDeveloper,
                                        std::move(contract), std::move(advance));
    PayrollPeriod period(2026, 10);
    CalculationContext ctx(worker.get(), period);

    Salary s(100000.0, 0.0, 100000.0);
    adv.apply(ctx, s);
    CHECK_CLOSE(70000.0, s.getNet(), kTolerance);
}

TEST(ProbationDiscountStrategyTest_AppliesDiscount) {
    ProbationDiscountStrategy prob;
    auto contract = makeContract(100000.0, "Dev");
    auto advance = makeAdvance(1000.0);
    auto worker = WorkerFactory::create(1, "Иван", WorkerType::SoftwareDeveloper,
                                        std::move(contract), std::move(advance));
    PayrollPeriod period(2026, 10);
    CalculationContext ctx(worker.get(), period);

    Salary s(100000.0, 0.0, 100000.0);
    prob.apply(ctx, s);
    CHECK_CLOSE(85000.0, s.getGross(), kTolerance);
    CHECK_CLOSE(85000.0, s.getNet(), kTolerance);
}
