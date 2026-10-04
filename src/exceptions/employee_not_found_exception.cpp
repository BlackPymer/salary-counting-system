#include "exceptions/employee_not_found_exception.hpp"

EmployeeNotFoundException::EmployeeNotFoundException(int employeeId,
                                                     const std::string& departmentName)
    : BaseException("Сотрудник с id=" + std::to_string(employeeId) + " не найден в отделе '" +
                    departmentName + "'"),
      employeeId_(employeeId),
      departmentName_(departmentName) {}

int EmployeeNotFoundException::getEmployeeId() const {
    return employeeId_;
}

const std::string& EmployeeNotFoundException::getDepartmentName() const {
    return departmentName_;
}
