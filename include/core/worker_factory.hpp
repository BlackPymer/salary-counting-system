#pragma once

#include <memory>
#include <string>

#include "workers/worker.hpp"

// Превращает подготовленные отделом кадров условия в сотрудника нужного
// типа. Контракт и аванс обязательны: сотрудник не может появиться без
// условий найма. Специфичные поля роли получают дефолтные значения,
// их можно поднять позже через типизированные аксессоры.
class WorkerFactory {
public:
    static std::unique_ptr<Worker> create(int id, const std::string& fullName, WorkerType type,
                                          std::unique_ptr<EmploymentContract> contract,
                                          std::unique_ptr<AdvancePayment> advance);

    static std::string positionFor(WorkerType type);
};
