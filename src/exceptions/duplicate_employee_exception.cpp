#include "exceptions/duplicate_employee_exception.hpp"

DuplicateEmployeeException::DuplicateEmployeeException(int employeeId, const std::string& fullName)
    : BaseException("Сотрудник с id=" + std::to_string(employeeId) + " ('" + fullName +
                    "') уже работает в компании"),
      employeeId_(employeeId),
      fullName_(fullName) {}

int DuplicateEmployeeException::getEmployeeId() const {
    return employeeId_;
}

const std::string& DuplicateEmployeeException::getFullName() const {
    return fullName_;
}
