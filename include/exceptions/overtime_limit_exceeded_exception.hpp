#pragma once

#include <string>

#include "base_exception.hpp"

class OvertimeLimitExceededException : public BaseException {
public:
    static constexpr int kErrorCode = 4009;
    int getErrorCode() const;

    OvertimeLimitExceededException(const std::string& fullName, double requestedHours,
                                   double limitHours);

    const std::string& getFullName() const;
    double getRequestedHours() const;
    double getLimitHours() const;

    ~OvertimeLimitExceededException() override = default;

private:
    std::string fullName_;
    double requestedHours_ = 0.0;
    double limitHours_ = 0.0;
};
