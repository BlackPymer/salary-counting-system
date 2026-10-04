#include "exceptions/overtime_limit_exceeded_exception.hpp"

OvertimeLimitExceededException::OvertimeLimitExceededException(const std::string& fullName,
                                                               double requestedHours,
                                                               double limitHours)
    : BaseException("Превышен лимит сверхурочных для '" + fullName +
                    "': " + std::to_string(static_cast<int>(requestedHours)) + " ч при нормативе " +
                    std::to_string(static_cast<int>(limitHours)) + " ч"),
      fullName_(fullName),
      requestedHours_(requestedHours),
      limitHours_(limitHours) {}

const std::string& OvertimeLimitExceededException::getFullName() const {
    return fullName_;
}

double OvertimeLimitExceededException::getRequestedHours() const {
    return requestedHours_;
}

double OvertimeLimitExceededException::getLimitHours() const {
    return limitHours_;
}
