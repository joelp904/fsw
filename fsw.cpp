#include "src/primitives/register.hpp"
#include "src/primitives/component.hpp"
#include "src/components/temperature_monitor.hpp"
#include "src/components/PCDU.hpp"
#include "src/simulation/thermal_sim.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>

int main()
{
    double SIM_INIT_TEMP = 24.0;
    int32_t SIM_TIME = 100;
    Temp_unit unit = FAHRENHEIT;
    std::string symbol = (unit == CELSIUS) ? "°C" : "°F";

    // Instantiate Components
    Temperature_monitor temp_monitor(unit);
    PCDU pcdu;
    Thermal_sim thermal_sim(temp_monitor, pcdu, SIM_INIT_TEMP);

    std::cout << "Initialize FSW\n";
    std::cout << "------------------------------------------------------------------------------\n";
    std::cout << "FSW: Simulation Initial Temperature: '" << SIM_INIT_TEMP << " " << symbol << "'\n";
    std::cout << "FSW: temp_monitor.set_min_temp_limit(THERMISTOR0, 0.0)" << "\n";
    temp_monitor.set_min_temp_limit(THERMISTOR0, 0.0);
    std::cout << "FSW: temp_monitor.set_max_temp_limit(THERMISTOR0, 25.0)" << "\n";
    temp_monitor.set_max_temp_limit(THERMISTOR0, 25.0);
    std::cout << "FSW: temp_monitor.get_min_temp_limit(THERMISTOR0): '"
              << std::fixed << std::setprecision(2) << temp_monitor.get_min_temp_limit(THERMISTOR0)
              << " " << symbol << "'\n";
    std::cout << "FSW: temp_monitor.get_max_temp_limit(THERMISTOR0): '"
              << std::fixed << std::setprecision(2) << temp_monitor.get_max_temp_limit(THERMISTOR0)
              << " " << symbol << "'\n";
    // Setup THERMISTOR1
    std::cout << "FSW: temp_monitor.set_min_temp_limit(THERMISTOR1, -10.0)" << "\n";
    temp_monitor.set_min_temp_limit(THERMISTOR1, -10.0);
    std::cout << "FSW: temp_monitor.set_max_temp_limit(THERMISTOR1, 40.0)" << "\n";
    temp_monitor.set_max_temp_limit(THERMISTOR1, 40.0);
    std::cout << "FSW: temp_monitor.get_min_temp_limit(THERMISTOR1): '"
              << std::fixed << std::setprecision(2) << temp_monitor.get_min_temp_limit(THERMISTOR1)
              << " " << symbol << "'\n";
    std::cout << "FSW: temp_monitor.get_max_temp_limit(THERMISTOR1): '"
              << std::fixed << std::setprecision(2) << temp_monitor.get_max_temp_limit(THERMISTOR1)
              <<  " " << symbol << "'\n";
    // Heater Status
    std::cout << "FSW: pcdu.get_status(HEATER1)= '" << pcdu.get_status(HEATER1) << "'\n";
    std::cout << "FSW: pcdu.get_status(HEATER1)= '" << pcdu.get_status(HEATER1) << "'\n";

    std::cout << "\nSimulation Start\n";
    std::cout << "------------------------------------------------------------------------------\n";
    std::cout << "FSW: temp_monitor.report_temps():\n";
    while (SIM_TIME != 0)
    {
        std::cout << temp_monitor.report_temps() << "\n";

        // HEATER0 Check
        if (temp_monitor.get_temp(THERMISTOR0) >= temp_monitor.get_max_temp_limit(THERMISTOR0))
        {
            std::cout << "\n    Disable HEATER0 Process\n";
            std::cout << "    ------------------------------------------------------------------------------\n";
            std::cout << "    FSW: temp_monitor.get_temp(THERMISTOR0)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_temp(THERMISTOR0)
                      << " " << symbol << "' \n";
            std::cout << "    FSW: temp_monitor.get_max_temp_limit()= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_max_temp_limit(THERMISTOR0)
                      << " " << symbol << "' reached\n";
            std::cout << "    FSW: pcdu.set_heater_enable(HEATER0, 0):\n";
            pcdu.set_heater_enable(HEATER0, 0);
            pcdu.get_status(HEATER0);
            std::cout << "    FSW: pcdu.get_status(HEATER0)= '" << pcdu.get_status(HEATER0) << "'\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
        if (temp_monitor.get_temp(THERMISTOR0) <= temp_monitor.get_min_temp_limit(THERMISTOR0))
        {
            std::cout << "\n    Enable HEATER0 Process\n";
            std::cout << "    ------------------------------------------------------------------------------\n";
            std::cout << "    FSW: temp_monitor.get_temp(THERMISTOR0)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_temp(THERMISTOR0)
                      << " " << symbol << "' \n";
            std::cout << "    FSW: temp_monitor.get_min_temp_limit(THERMISTOR0)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_min_temp_limit(THERMISTOR0)
                      << " " << symbol << "' reached\n";
            std::cout << "    FSW: pcdu.set_heater_enable(HEATER0, 1)\n";
            pcdu.set_heater_enable(HEATER0, 1);
            std::cout << "    FSW: pcdu.get_status(HEATER0)= '" << pcdu.get_status(HEATER0) << "'\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }

        // HEATER1 Check
        if (temp_monitor.get_temp(THERMISTOR1) >= temp_monitor.get_max_temp_limit(THERMISTOR1))
        {
            std::cout << "\n    Disable HEATER1 Process\n";
            std::cout << "    ------------------------------------------------------------------------------\n";
            std::cout << "    FSW: temp_monitor.get_temp(THERMISTOR1)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_temp(THERMISTOR1)
                      << " " << symbol << "' \n";
            std::cout << "    FSW: temp_monitor.get_max_temp_limit()= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_max_temp_limit(THERMISTOR1)
                      << " " << symbol << "' reached\n";
            std::cout << "    FSW: pcdu.set_heater_enable(HEATER1, 0):\n";
            pcdu.set_heater_enable(HEATER1, 0);
            pcdu.get_status(HEATER1);
            std::cout << "    FSW: pcdu.get_status(HEATER1)= '" << pcdu.get_status(HEATER1) << "'\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
        if (temp_monitor.get_temp(THERMISTOR1) <= temp_monitor.get_min_temp_limit(THERMISTOR1))
        {
            std::cout << "\n    Enable HEATER1 Process\n";
            std::cout << "    ------------------------------------------------------------------------------\n";
            std::cout << "    FSW: temp_monitor.get_temp(THERMISTOR1)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_temp(THERMISTOR1)
                      << " " << symbol << "' \n";
            std::cout << "    FSW: temp_monitor.get_min_temp_limit(THERMISTOR1)= '"
                      << std::fixed << std::setprecision(2) << temp_monitor.get_min_temp_limit(THERMISTOR1)
                      << " " << symbol << "' reached\n";
            std::cout << "    FSW: pcdu.set_heater_enable(HEATER1, 1)\n";
            pcdu.set_heater_enable(HEATER1, 1);
            std::cout << "    FSW: pcdu.get_status(HEATER1)= '" << pcdu.get_status(HEATER1) << "'\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }

        SIM_TIME --;
        thermal_sim.tick();
    }
    std::cout << "\n";
}
