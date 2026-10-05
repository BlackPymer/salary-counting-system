#pragma once

#include <memory>
#include <string>

#include "workers/worker.hpp"

class WorkerFactory {
public:
    static std::unique_ptr<Worker> create(int id, const std::string& fullName, WorkerType type,
                                          std::unique_ptr<EmploymentContract> contract,
                                          std::unique_ptr<AdvancePayment> advance);

    static std::string positionFor(WorkerType type);
};
