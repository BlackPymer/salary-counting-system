#include "workers/manager.hpp"

#include "departments/department.hpp"

std::string Manager::getRole() const {
    return "Manager";
}

std::string Manager::generateReport() const {
    std::string report = "Отчёт руководителя: " + getFullName() + " (" + getPosition() + ")";
    const Department* department = getDepartment();
    if (department != nullptr) {
        report += ", в подчинении " + std::to_string(department->getWorkersCount()) + " чел.";
    }
    return report;
}

bool Manager::approveLeave(int employeeId) {
    const Department* department = getDepartment();
    if (department == nullptr || employeeId == getId()) {
        return false;
    }
    return department->findWorker(employeeId) != nullptr;
}

double Manager::calculateRoleBonus() const {
    return getBaseRate() * managementBonusRate_;
}
