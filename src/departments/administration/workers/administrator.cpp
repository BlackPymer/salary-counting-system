#include "departments/administration/workers/administrator.hpp"

namespace {
constexpr double kOfficeAdminBonus = 0.05;
constexpr int kBonusTaskThreshold = 10;
}  // namespace

Administrator::Administrator(int id, std::string fullName,
                             std::unique_ptr<EmploymentContract> contract,
                             std::unique_ptr<AdvancePayment> advance, std::string officeBuilding)
    : Manager(id, std::move(fullName), std::move(contract), std::move(advance)),
      officeBuilding_(std::move(officeBuilding)) {}

std::string Administrator::getRole() const {
    return "Administrator";
}

const std::string& Administrator::getOfficeBuilding() const {
    return officeBuilding_;
}

int Administrator::getTasksCompleted() const {
    return tasksCompleted_;
}

void Administrator::completeTask() {
    ++tasksCompleted_;
}

double Administrator::calculateRoleBonus() const {
    const double progressBonus =
        tasksCompleted_ >= kBonusTaskThreshold ? kOfficeAdminBonus : kOfficeAdminBonus / 2.0;
    return Manager::calculateRoleBonus() + getBaseRate() * progressBonus;
}
