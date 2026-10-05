#include <UnitTest++/UnitTest++.h>

#include "core/worker_factory.hpp"
#include "financial_objects/payroll_period.hpp"
#include "payroll/calculation_context.hpp"
#include "test_helpers.hpp"
#include "workers/worker.hpp"

namespace {
constexpr double kTolerance = 0.001;
}

TEST(CalculationContextTest_BasicProperties) {
    auto contract = makeContract(100000.0, "Разработчик");
    auto advance = makeAdvance(30000.0);
    auto worker = WorkerFactory::create(1, "Иван Иванов", WorkerType::SoftwareDeveloper,
                                        std::move(contract), std::move(advance));
    PayrollPeriod period(2026, 10);
    CalculationContext ctx(worker.get(), period);

    CHECK_CLOSE(100000.0, ctx.getBaseRate(), kTolerance);
    CHECK_CLOSE(100000.0 * 0.85, ctx.getEffectiveRate(), kTolerance);
    CHECK(ctx.isOnProbation());
    CHECK(ctx.isActive());
    CHECK_EQUAL(10, ctx.getPeriod().getMonth());
    CHECK_EQUAL(period.getLastDay(), ctx.getCalculationDate());
}

TEST(SalaryCalculation_IsIdempotent) {
    auto contract = makeContract(100000.0, "Разработчик");
    auto advance = makeAdvance(30000.0);
    Worker worker(1, "Иван", std::move(contract), std::move(advance));

    const Salary first = worker.calculateSalary();
    const Salary second = worker.calculateSalary();
    const Salary third = worker.calculateSalary();

    CHECK_CLOSE(first.getGross(), second.getGross(), kTolerance);
    CHECK_CLOSE(first.getNet(), second.getNet(), kTolerance);
    CHECK_CLOSE(first.getTaxDeduction(), second.getTaxDeduction(), kTolerance);
    CHECK_CLOSE(first.getGross(), third.getGross(), kTolerance);
    CHECK_CLOSE(first.getNet(), third.getNet(), kTolerance);
    CHECK_CLOSE(first.getTaxDeduction(), third.getTaxDeduction(), kTolerance);
}

TEST(SalaryCalculation_AdvanceNotMutatedByCalculation) {
    auto contract = makeContract(100000.0, "Разработчик");
    auto advance = makeAdvance(30000.0);
    Worker worker(1, "Иван", std::move(contract), std::move(advance));

    worker.calculateSalary();
    worker.calculateSalary();

    CHECK_CLOSE(30000.0, worker.getAdvance().getRemaining(), kTolerance);
    CHECK(!worker.getAdvance().isFullyRepaid());
}

TEST(AdvanceRepayment_DeductsFromAvailable) {
    auto contract = makeContract(100000.0, "Разработчик");
    auto advance = makeAdvance(30000.0);
    Worker worker(1, "Иван", std::move(contract), std::move(advance));

    CHECK_CLOSE(20000.0, worker.repayAdvance(20000.0), kTolerance);
    CHECK_CLOSE(10000.0, worker.getAdvance().getRemaining(), kTolerance);

    CHECK_CLOSE(10000.0, worker.repayAdvance(20000.0), kTolerance);
    CHECK_CLOSE(0.0, worker.getAdvance().getRemaining(), kTolerance);
    CHECK(worker.getAdvance().isFullyRepaid());

    CHECK_CLOSE(0.0, worker.repayAdvance(50000.0), kTolerance);
}

TEST(AdvanceReset_ClearsRemainingBalance) {
    auto contract = makeContract(100000.0, "Разработчик");
    auto advance = makeAdvance(30000.0);
    Worker worker(1, "Иван", std::move(contract), std::move(advance));

    worker.repayAdvance(10000.0);
    CHECK_CLOSE(20000.0, worker.getAdvance().getRemaining(), kTolerance);

    worker.resetAdvance();
    CHECK_CLOSE(0.0, worker.getAdvance().getRemaining(), kTolerance);
    CHECK(worker.getAdvance().isFullyRepaid());
}
