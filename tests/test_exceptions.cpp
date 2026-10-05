#include <UnitTest++/UnitTest++.h>

#include "exceptions/base_exception.hpp"
#include "exceptions/contract_expired_exception.hpp"
#include "exceptions/file_io_exception.hpp"
#include "exceptions/insufficient_funds_exception.hpp"
#include "exceptions/payment_failed_exception.hpp"
#include "exceptions/salary_calculation_exception.hpp"
#include "exceptions/tax_calculation_exception.hpp"
#include "exceptions/unauthorized_access_exception.hpp"

TEST(BaseInheritance_SalaryCalculation) {
    try {
        throw SalaryCalculationException("calc error");
    } catch (const SalaryCalculationException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_TaxCalculation) {
    try {
        throw TaxCalculationException("tax error");
    } catch (const TaxCalculationException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_ContractExpired) {
    try {
        throw ContractExpiredException("expired");
    } catch (const ContractExpiredException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_InsufficientFunds) {
    try {
        throw InsufficientFundsException("funds");
    } catch (const InsufficientFundsException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_PaymentFailed) {
    try {
        throw PaymentFailedException("pay");
    } catch (const PaymentFailedException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_UnauthorizedAccess) {
    try {
        throw UnauthorizedAccessException("auth");
    } catch (const UnauthorizedAccessException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(BaseInheritance_FileIo) {
    try {
        throw FileIoException("io");
    } catch (const FileIoException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}

TEST(CatchAsBase_InsufficientFunds) {
    try {
        throw InsufficientFundsException("insufficient");
    } catch (const BaseException&) {
        CHECK(true);
        return;
    } catch (...) {
    }
    CHECK(false);
}
