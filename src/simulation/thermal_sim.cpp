#include <iostream>
#include <chrono>
#include <thread>

#include "thermal_sim.hpp"
#include "../components/temperature_monitor.hpp"
#include "../components/PCDU.hpp"

Thermal_sim::Thermal_sim (
    Temperature_monitor& temperature_monitor,
    PCDU& pcdu,
    double initial_temperature
)
:
temperature_monitor_(temperature_monitor),
pcdu_(pcdu),
area0_temperature_(initial_temperature),
area1_temperature_(initial_temperature)
{
    // Set initial temperatures of simulation model
    temperature_monitor_.sim_temp_update(THERMISTOR0, area0_temperature_);
    temperature_monitor_.sim_temp_update(THERMISTOR1, area1_temperature_);
}

void Thermal_sim::tick()
{
    // Area0 Temperature / Heater Simulation
    if (pcdu_.sim_heater_powered(HEATER0))
    {
        area0_temperature_++;
    }
    else
    {
        area0_temperature_--;
    }
    temperature_monitor_.sim_temp_update(THERMISTOR0, area0_temperature_);

    // Area1 Temperature / Heater Simulation
    if (pcdu_.sim_heater_powered(HEATER1))
    {
        area1_temperature_ = area1_temperature_ + 2;
    }
    else
    {
        area1_temperature_ = area1_temperature_ - 2;
    }
    temperature_monitor_.sim_temp_update(THERMISTOR1, area1_temperature_);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(350)
    );
}

