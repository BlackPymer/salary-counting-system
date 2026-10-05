#include <UnitTest++/UnitTest++.h>

#include "core/worker_factory.hpp"
#include "financial_objects/payroll_period.hpp"
#include "payroll/calculation_context.hpp"
#include "test_helpers.hpp"

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
