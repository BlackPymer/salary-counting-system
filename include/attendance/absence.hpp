#pragma once

#include <memory>
#include <string>

class Absence {
public:
    static constexpr double kFullPay = 1.0;
    static constexpr double kNoPay = 0.0;
    static constexpr double kHalfPay = 0.5;

    Absence(int days, double payRate);
    virtual ~Absence() = default;

    Absence(const Absence&) = delete;
    Absence& operator=(const Absence&) = delete;
    Absence(Absence&&) = delete;
    Absence& operator=(Absence&&) = delete;

    static std::unique_ptr<Absence> dayOff(int days);

    int getDays() const;

    virtual double getPayRate() const;

    virtual std::string getTypeName() const;

protected:
    int days_ = 0;
    double payRate_ = kNoPay;
};
