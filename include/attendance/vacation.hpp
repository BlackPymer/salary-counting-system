#pragma once

#include <memory>
#include <string>

#include "absence.hpp"

// Отпуск. Оплачиваемый и без сохранения з/п — один класс, различается
// только ставка оплаты. Отгук — тоже Vacation с частичной ставкой.
class Vacation : public Absence {
public:
    Vacation(int days, bool isPaid);
    ~Vacation() override = default;

    static std::unique_ptr<Vacation> paid(int days);
    static std::unique_ptr<Vacation> unpaid(int days);

    std::string getTypeName() const override;
};
