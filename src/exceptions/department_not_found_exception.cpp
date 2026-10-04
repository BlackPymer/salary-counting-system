#include "exceptions/department_not_found_exception.hpp"

DepartmentNotFoundException::DepartmentNotFoundException(const std::string& departmentName)
    : BaseException("Отдел '" + departmentName + "' не найден"), departmentName_(departmentName) {}

const std::string& DepartmentNotFoundException::getDepartmentName() const {
    return departmentName_;
}
