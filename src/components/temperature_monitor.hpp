#pragma once

#include "../primitives/component.hpp"

enum Temp_unit
{
    CELSIUS,
    FAHRENHEIT
};

enum Thermistor_num
{
    THERMISTOR0,
    THERMISTOR1
};

struct Thermistor_temps
{
    double thermistor0;
    double thermistor1;
};

class Temperature_monitor : public Component
{
public:
    Temperature_monitor(Temp_unit unit = CELSIUS);

    double get_temp(Thermistor_num thermistor) const;
    std::string report_temps() const; 

    void set_min_temp_limit(Thermistor_num thermistor, double temp);
    double get_min_temp_limit(Thermistor_num thermistor) const;

    void set_max_temp_limit(Thermistor_num thermistor, double temp);
    double get_max_temp_limit(Thermistor_num thermistor) const;

protected:
    void validate_write_reg(uint32_t address, uint32_t value) const override;

private:
    // THERMISTOR0
    static constexpr uint32_t TEMP0_ADDR            = 0x00000000;
    static constexpr uint32_t TEMP0_MIN_LIMIT_ADDR  = 0x00000004;
    static constexpr uint32_t TEMP0_MAX_LIMIT_ADDR  = 0x00000008;
    // THERMISTOR1
    static constexpr uint32_t TEMP1_ADDR            = 0x0000000C;
    static constexpr uint32_t TEMP1_MIN_LIMIT_ADDR  = 0x00000010;
    static constexpr uint32_t TEMP1_MAX_LIMIT_ADDR  = 0x00000014;

    double celsius_to_farh(double temp) const;
    double farh_to_celsius(double temp) const;

    // Simulation Only
    friend class Thermal_sim;
    void sim_temp_update(Thermistor_num thermistor, double temp);

    // Member Variables
    Temp_unit unit_;
};
