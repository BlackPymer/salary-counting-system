#include "departments/security_department.hpp"

SecurityDepartment::SecurityDepartment()
    : Department("Служба безопасности",
                 "Охрана объектов, контроль доступа, безопасность информации", kMaxHeadcount,
                 kMonthlyBudget) {}
