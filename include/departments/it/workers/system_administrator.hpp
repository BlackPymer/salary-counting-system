#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/specialist.hpp"

class SystemAdministrator : public Specialist {
public:
    static constexpr double kBaseSysadminRate = 0.11;
    static constexpr double kUptimeBonus = 0.09;
    static constexpr double kCriticalDowntimePercent = 99.0;

    SystemAdministrator(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                        std::unique_ptr<AdvancePayment> advance, int serversMaintained);

    std::string getRole() const override;
    std::string getSpecialization() const override;

    int getServersMaintained() const;
    double getUptimePercent() const;
    void recordUptime(double percent);
    void addServer();

    bool hasCriticalDowntime() const;

protected:
    double calculateRoleBonus() const override;

private:
    int serversMaintained_ = 0;
    double uptimePercent_ = 100.0;
};
