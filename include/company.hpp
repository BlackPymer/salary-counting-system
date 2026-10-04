#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "departments/department.hpp"
#include "workers/worker.hpp"

class Recruiter;

// Компания владеет отделами. Отделы владеют сотрудниками.
//
// Company — фасад: единственная точка найма. Условия найма готовит отдел
// кадров, объект сотрудника создаёт WorkerFactory, владение объектом
// переходит к отделу.
class Company {
public:
    explicit Company(std::string name);

    void addDepartment(std::unique_ptr<Department> department);
    Department* findDepartment(const std::string& name) const;
    std::size_t getDepartmentsCount() const;

    // Первичное назначение рекрутера. Компания заводит его сама, потому что
    // через отдел кадров найм невозможен: пока рекрутера нет, нанять некого.
    Worker* appointRecruiter(const std::string& fullName, double monthlyRate);

    // Аванс при найме считает сам отдел кадров: hireWorker() передаёт только
    // ставку, а размер аванса определяется правилами HR.
    Worker* hireWorker(const std::string& departmentName, const std::string& fullName,
                       WorkerType type, double monthlyRate);

    std::unique_ptr<Worker> terminateWorker(const std::string& departmentName, int workerId);

    // Команды симуляции. Симуляция живёт снаружи и сообщает компании факты:
    // кто сколько дней отсутствовал и по какой причине.
    void registerAbsence(const std::string& departmentName, int workerId,
                         std::unique_ptr<Absence> absence);
    void registerOvertime(const std::string& departmentName, int workerId, double hours);

    // Завершение расчётного периода: сбрасывает накопленные факты,
    // чтобы они не попали в следующий месяц.
    void endPeriod();

    double calculateCompanyPayroll() const;
    int getTotalWorkersCount() const;

    std::string generateReport() const;

private:
    Recruiter* findRecruiter() const;
    Worker* findWorkerIn(const std::string& departmentName, int workerId) const;

    std::string name_;
    std::vector<std::unique_ptr<Department>> departments_;
    int nextWorkerId_ = 1;
};
