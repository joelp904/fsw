#include <cstdint>

class Temperature_monitor;
class PCDU;

class Thermal_sim {
public:
    Thermal_sim (
        Temperature_monitor& temperature_monitor,
        PCDU& pcdu,
        double initial_temperature
    );

    void tick();

private:
    Temperature_monitor& temperature_monitor_;
    PCDU& pcdu_;
    double area0_temperature_;
    double area1_temperature_;
};
