#include "departments/it/workers/system_administrator.hpp"

#include "exceptions/invalid_input_exception.hpp"

SystemAdministrator::SystemAdministrator(int id, std::string fullName,
                                         std::unique_ptr<EmploymentContract> contract,
                                         std::unique_ptr<AdvancePayment> advance,
                                         int serversMaintained)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      serversMaintained_(serversMaintained) {
    if (serversMaintained_ < 0) {
        throw InvalidInputException("Количество серверов не может быть отрицательным");
    }
}

std::string SystemAdministrator::getRole() const {
    return "SystemAdministrator";
}

std::string SystemAdministrator::getSpecialization() const {
    return "Администрирование инфраструктуры";
}

int SystemAdministrator::getServersMaintained() const {
    return serversMaintained_;
}

double SystemAdministrator::getUptimePercent() const {
    return uptimePercent_;
}

void SystemAdministrator::recordUptime(double percent) {
    if (percent < 0.0 || percent > 100.0) {
        throw InvalidInputException("Uptime должен быть в диапазоне [0, 100]");
    }
    uptimePercent_ = percent;
}

void SystemAdministrator::addServer() {
    ++serversMaintained_;
}

bool SystemAdministrator::hasCriticalDowntime() const {
    return uptimePercent_ < kCriticalDowntimePercent;
}

double SystemAdministrator::calculateRoleBonus() const {
    return getBaseRate() * (kBaseSysadminRate + (hasCriticalDowntime() ? 0.0 : kUptimeBonus));
}
