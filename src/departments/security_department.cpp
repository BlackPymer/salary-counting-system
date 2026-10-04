#include "departments/security_department.hpp"

SecurityDepartment::SecurityDepartment() : Department("Служба безопасности") {}

std::string SecurityDepartment::getDescription() const {
    return "Охрана объектов, контроль доступа, безопасность информации";
}
