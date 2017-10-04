#include "luaauto_global.h"
#include "auto.h"

Auto::Auto()
    : m_brand("Audi!"), m_mileage(0),  m_hours(0), m_fuel(0)
{
    std::cout << "Auto::Auto()" << std::endl;
}

Auto::~Auto()
{
    std::cout << "Auto::~Auto()" << std::endl;
}

void Auto::drive(const std::string& driver, int mileage, double hours)
{
    const int oilConsumptionRate = 6;

    mileage = std::min(m_fuel * oilConsumptionRate, mileage);
    std::cout<< "Drived about " << hours << "hours, and " << mileage << " miles by "
              << driver << std::endl;

    m_mileage += mileage;
    m_fuel -= (mileage / oilConsumptionRate);
    m_hours += hours;
}

void Auto::fuel(int quantity)
{
    m_fuel += quantity;
}

int Auto::maintain(const std::string& something)
{
    std::cout << "maintain with " << something << std::endl;
    return m_mileage * 2 + m_hours * 0.1;
}

std::tuple<std::string, int, float, int> Auto::check() const
{
    std::cout << "check: " << m_brand <<", " << m_mileage << ", " << m_hours << ", " << m_fuel << std::endl;
    return std::make_tuple(m_brand, m_mileage, m_hours, m_fuel);
}

Car::Car()
    : Auto()
{
    std::cout << "Car::Car()" << std::endl;
}

Car::~Car()
{
    std::cout << "Car::~Car()" << std::endl;
}

double Car::music(const std::string &song)
{
    std::cout << "music: song ... " << song << std::endl;
    return song.length() * 1.234;
}

bool Car::navi(const std::string &address)
{
    std::cout << "navi: address is " << address << std::endl;
    return (address.length() % 2) == 1;
}

Tesla::Tesla()
    : Car()
{
    std::cout << "Tesla::Tesla()" << std::endl;
}

Tesla::~Tesla()
{
    std::cout << "Tesla::~Tesla()" << std::endl;
}

int Tesla::charge(double d)
{
    int i = int(d * 3.3333);
    std::cout << "charge about " << d << " degree and return " << i << std::endl;
    return i;
}

void Tesla::bluetooth(const std::string &name)
{
    std::cout << "bluetooth: " << name << std::endl;
}

AirVehicle::AirVehicle()
{
    std::cout << "AirVehicle::AirVehicle()" << std::endl;
}

AirVehicle::~AirVehicle()
{
    std::cout << "AirVehicle::~AirVehicle()" << std::endl;
}

int AirVehicle::fly(double speed)
{
    int i = int(speed * 5.5555);
    std::cout << "I am flying about " << i << std::endl;
    return i;
}

Seafaring::Seafaring()
{
    std::cout << "Seafaring::Seafaring()" << std::endl;
}

Seafaring::~Seafaring()
{
    std::cout << "Seafaring::~Seafaring()" << std::endl;
}

int Seafaring::voyage(double speed)
{
    int i = int(speed * 7.8888);
    std::cout << "voyage: " << i << std::endl;
    return i;
}

std::string Seafaring::dive(double deep, int hours)
{
    std::stringstream ss;
    ss << "Seafaring::dive: deep = " << deep << ", hours = " << hours << std::endl;
    std::cout << ss.str();
    return ss.str();
}

SuperCar::SuperCar()
{
    std::cout << "SuperCar::SuperCar()" << std::endl;
}

SuperCar::~SuperCar()
{
    std::cout << "SuperCar::~SuperCar()" << std::endl;
}

void SuperCar::stealth()
{
    std::cout << "i am stealthing now!!!" << std::endl;
}

std::string SuperCar::dive(double deep, int hours)
{
    std::stringstream ss;
    ss << "SuperCar::dive: deep = " << deep << ", hours = " << hours << std::endl;
    std::cout << ss.str();
    return ss.str();
}
