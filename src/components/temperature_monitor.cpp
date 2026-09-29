#include <stdexcept>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>

#include "temperature_monitor.hpp"

namespace
{
// TODO: Understand why we use this over static_int32t<>
// Represents temperature (double) -> reg val 'raw' (uint32_t) conversion
uint32_t encode_temp(double temp)
{
    const int32_t millidegrees = static_cast<int32_t>(std::lround((temp * 1000.0)));

    uint32_t raw;
    std::memcpy(&raw, &millidegrees, sizeof(raw));
    return raw;
}

// Represents reg val 'raw' (uint32_t) -> temperature (int32_t) conversion
double decode_temp(uint32_t raw)
{
    int32_t convert;
    std::memcpy(&convert, &raw, sizeof(convert));
    return convert / 1000.0;
}

//// TODO: Understand why we use this over static_int32t<>
//// Represents temperature (int32_t) -> reg val 'raw' (uint32_t) conversion
//uint32_t encode_temp(int32_t value)
//{
//    uint32_t raw;
//    std::memcpy(&raw, &value, sizeof(raw));
//    return raw;
//}

//// Represents reg val 'raw' (uint32_t) -> temperature (int32_t) conversion
//int32_t decode_temp(uint32_t raw)
//{
//    int32_t value;
//    std::memcpy(&value, &raw, sizeof(value));
//    return value;
//}
} // end namespace

Temperature_monitor::Temperature_monitor(Temp_unit unit)
:
Component("temperature_monitor"),
unit_(unit)
{
    // THERMISTOR0
    add_reg(
        "THERMISTOR0_TEMP",
        TEMP0_ADDR,
        READ_ONLY,
        encode_temp(0)
    );

    add_reg(
        "THERMISTOR0_MIN_LIMIT",
        TEMP0_MIN_LIMIT_ADDR,
        READ_WRITE,
        encode_temp(0)
    );

    add_reg(
        "THERMISTOR0_MAX_LIMIT",
        TEMP0_MAX_LIMIT_ADDR,
        READ_WRITE,
        encode_temp(40)
    );
    // THERMISTOR1
    add_reg(
        "THERMISTOR1_TEMP",
        TEMP1_ADDR,
        READ_ONLY,
        encode_temp(0)
    );

    add_reg(
        "THERMISTOR1_MIN_LIMIT",
        TEMP1_MIN_LIMIT_ADDR,
        READ_WRITE,
        encode_temp(0)
    );

    add_reg(
        "THERMISTOR1_MAX_LIMIT",
        TEMP1_MAX_LIMIT_ADDR,
        READ_WRITE,
        encode_temp(40)
    );
}

double Temperature_monitor::celsius_to_farh(double temp) const
{
    return (temp * 9.0 / 5.0) + 32.0;
}

double Temperature_monitor::farh_to_celsius(double temp) const
{
    return (temp - 32.0) * 5.0 / 9.0;
}


// PCDU API
// -------------------------------------------------------------------------------------------------
double Temperature_monitor::get_temp(Thermistor_num thermistor) const
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_ADDR : TEMP1_ADDR;

    double temp = decode_temp(read_reg(reg));
    if (unit_ == FAHRENHEIT)
    {
        temp = celsius_to_farh(temp);
    }
    return temp;
}

std::string Temperature_monitor::report_temps() const
{
    std::string report;
    Thermistor_temps temps;
    temps.thermistor0 = get_temp(THERMISTOR0);
    temps.thermistor1 = get_temp(THERMISTOR1);

    report =  "THERMISTOR0 Temp: '" + std::to_string(temps.thermistor0) + "' " +
              "THERMISTOR1 Temp: '" + std::to_string(temps.thermistor1) + "' ";

    return report;
}

void Temperature_monitor::set_min_temp_limit(Thermistor_num thermistor, double temp)
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_MIN_LIMIT_ADDR : TEMP1_MIN_LIMIT_ADDR;

    if (unit_ == FAHRENHEIT)
    {
        temp = farh_to_celsius(temp);
    }
    write_reg(reg, encode_temp(temp));
}

double Temperature_monitor::get_min_temp_limit(Thermistor_num thermistor) const
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_MIN_LIMIT_ADDR : TEMP1_MIN_LIMIT_ADDR;

    double temp = decode_temp(read_reg(reg));
    if (unit_ == FAHRENHEIT)
    {
        temp = celsius_to_farh(temp);
    }
    return temp;
}

void Temperature_monitor::set_max_temp_limit(Thermistor_num thermistor, double temp)
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_MAX_LIMIT_ADDR : TEMP1_MAX_LIMIT_ADDR;
    if (unit_ == FAHRENHEIT)
    {
        temp = farh_to_celsius(temp);
    }
    write_reg(reg, encode_temp(temp));
}

double Temperature_monitor::get_max_temp_limit(Thermistor_num thermistor) const
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_MAX_LIMIT_ADDR : TEMP1_MAX_LIMIT_ADDR;

    double temp = decode_temp(read_reg(reg));
    if (unit_ == FAHRENHEIT)
    {
        temp = celsius_to_farh(temp);
    }
    return temp;

}

void Temperature_monitor::validate_write_reg(uint32_t address, uint32_t raw_value) const
{
    const double value = decode_temp(raw_value);

    // Validate MIN_LIMIT write
    if (address == TEMP0_MIN_LIMIT_ADDR || address == TEMP1_MIN_LIMIT_ADDR)
    {
        const uint32_t max_address = (address == TEMP0_MIN_LIMIT_ADDR)
                             ? TEMP0_MAX_LIMIT_ADDR
                             : TEMP1_MAX_LIMIT_ADDR;

        const double max_temp = decode_temp(read_reg(max_address));

        if (value > max_temp)
        {
            throw std::runtime_error(
                "Minimum temperature '" + std::to_string(value) +
                "' cannot exceed maximum temperature '" +
                std::to_string(max_temp) + "'\n"
            );
        }
    }
    // Validate MAX_LIMIT_WRITE
    else if (address == TEMP0_MAX_LIMIT_ADDR || address == TEMP1_MAX_LIMIT_ADDR)
    {
        const uint32_t min_address = (address == TEMP0_MAX_LIMIT_ADDR)
                             ? TEMP0_MIN_LIMIT_ADDR
                             : TEMP1_MIN_LIMIT_ADDR;

        const double min_temp = decode_temp(read_reg(min_address));

        if (value < min_temp)
        {
            throw std::runtime_error(
                "Minimum temperature '" + std::to_string(value) +
                "' cannot exceed maximum temperature '" +
                std::to_string(min_temp) + "'\n"
            );
        }
    }
}

// Temperature Monitor Hardware Simulation Interface
// -------------------------------------------------------------------------------------------------
void Temperature_monitor::sim_temp_update(Thermistor_num thermistor, double temp)
{
    uint32_t reg = (thermistor == THERMISTOR0) ? TEMP0_ADDR : TEMP1_ADDR;
    if (unit_ == FAHRENHEIT)
    {
        temp = farh_to_celsius(temp);
    }
    if (thermistor == THERMISTOR0)
    {
        sim_wreg_from_hardware(reg, encode_temp(temp));
    }
    else
    {
        sim_wreg_from_hardware(reg, encode_temp(temp));
    }
}

// Temperature_monitor testing:
//    std::cout << "Test Temperature_monitor\n";
//    std::cout << "------------------------------------------------------------------------------\n";
//    std::cout << "TEST: Temperature_monitor instantiation:\n";
//    Temperature_monitor temp_monitor;
//    std::cout << "    DEFAULT temp_monitor.get_temp()= " << temp_monitor.get_temp() << "\n";
//    std::cout << "    DEFAULT temp_monitor.get_min_temp_limit()= "
//              << temp_monitor.get_min_temp_limit() << "\n";
//    std::cout << "    DEFAULT temp_monitor.get_max_temp_limit()= "
//              << temp_monitor.get_max_temp_limit() << "\n\n";
//
//    std::cout << "TEST: set_min_temp_limit() / get_min_temp_limit():\n";
//    std::cout << "    temp_monitor.set_min_temp_limit(-1)\n";
//    temp_monitor.set_min_temp_limit(-1);
//    std::cout << "    temp_monitor.get_min_temp_limit()= "
//              << temp_monitor.get_min_temp_limit() << "\n\n";
//
//    std::cout << "TEST: set_max_temp_limit() / get_max_temp_limit():\n";
//    std::cout << "    temp_monitor.set_max_temp_limit(200)\n";
//    temp_monitor.set_max_temp_limit(200);
//    std::cout << "    temp_monitor.get_max_temp_limit()= "
//              << temp_monitor.get_max_temp_limit() << "\n\n";
//
//    std::cout << "TEST: Test max < min: temp_monitor.set_max_temp_limit(-10)\n";
//    try
//    {
//        temp_monitor.set_max_temp_limit(-10);
//    }
//    catch (const std::runtime_error& e)
//    {
//        std::cout << "    PASS: Caught Error: " << e.what() << "\n";
//    }
//
//    std::cout << "TEST: Test min > max: temp_monitor.set_min_temp_limit(-10)\n";
//    try
//    {
//        temp_monitor.set_min_temp_limit(300);
//    }
//    catch (const std::runtime_error& e)
//    {
//        std::cout << "    PASS: Caught Error: " << e.what() << "\n";
//    }
//
//    return 0;
