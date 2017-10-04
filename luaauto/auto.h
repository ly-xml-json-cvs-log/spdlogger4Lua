#ifndef AUTO_H
#define AUTO_H


class Auto
{
    std::string m_brand;
    int m_mileage;
    double m_hours;
    int m_fuel;

public:
    Auto();
    virtual ~Auto();

    virtual void drive(const std::string& driver, int mileage, double hours);
    void fuel(int quantity);
    virtual int maintain(const std::string& something);
    std::tuple<std::string, int, float, int> check() const;
};

class Car: public Auto
{
public:
    Car();
    virtual ~Car();

    double music(const std::string& song);
    bool navi(const std::string& address);
};

class Tesla : public Car
{
public:
    Tesla();
    virtual ~Tesla();

    int charge(double d);
    void bluetooth(const std::string& name);
};

class AirVehicle
{
public:
    AirVehicle();
    virtual ~AirVehicle();

    virtual int fly(double speed);
};

class Seafaring
{
public:
    Seafaring();
    virtual ~Seafaring();

    virtual int voyage(double speed);
    virtual std::string dive(double deep, int hours);
};

class SuperCar: public Tesla
{
public:
    SuperCar();
    virtual ~SuperCar();

    void stealth();
    virtual std::string dive(double deep, int hours);
};

#endif // AUTO_H
