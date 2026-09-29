#include <iostream>

#include "PCDU.hpp"

PCDU::PCDU()
:
Component("PCDU")
{
    // PCDU Register Definitions
    add_reg(
        "HEATER0_PWR",
        HEATER0_PWR_ADDR,
        WRITE_ONLY,
        0
    );
    add_reg(
        "HEATER1_PWR",
        HEATER1_PWR_ADDR,
        WRITE_ONLY,
        0
    );
}

// PCDU API
// -------------------------------------------------------------------------------------------------
void PCDU::set_heater_enable(Heater_num heater, bool enable)
{
    if (heater == HEATER0)
    {
        write_reg(HEATER0_PWR_ADDR, enable ? 1U : 0U);
        heater0_is_powered_ = enable;
    }
    else
    {
        write_reg(HEATER1_PWR_ADDR, enable ? 1U : 0U);
        heater1_is_powered_ = enable;
    }
}

std::string PCDU::get_status(Heater_num heater) const
{
    if (heater == HEATER0)
    {
        return heater0_is_powered_ ? "HEATER0: is ON"
                                   : "HEATER0: is OFF";
    }
    else
    {
        return heater1_is_powered_ ? "HEATER1: is ON"
                                   : "HEATER1: is OFF";
    }
}

// PCDU Hardware Simulation Interface
// -------------------------------------------------------------------------------------------------
bool PCDU::sim_heater_powered(Heater_num heater) const
{
    if (heater == HEATER0)
    {
        if (sim_rreg_to_hardware(HEATER0_PWR_ADDR) == 1U)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (sim_rreg_to_hardware(HEATER1_PWR_ADDR) == 1U)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

//    // PCDU testing:
//    std::cout << "Test PCDU\n";
//    std::cout << "------------------------------------------------------------------------------\n";
//    std::cout << "TEST: Temperature_monitor instantiation:\n";
//    PCDU pcdu;
//    std::cout << "    DEFAULT pcdu.get_status()= " << pcdu.get_status() << "\n\n";
//
//    std::cout << "TEST: Enable heater pcdu.set_heater_enable(1):\n";
//    pcdu.set_heater_enable(1);
//    std::cout << "    pcdu.enable_heater()\n";
//    std::cout << "    pcdu.get_status()= " << pcdu.get_status() << "\n\n";
//
//    std::cout << "TEST: Disable heater pcdu.set_heater_enable(0):\n";
//    pcdu.set_heater_enable(0);
//    std::cout << "    pcdu.enable_heater()\n";
//    std::cout << "    pcdu.get_status()= " << pcdu.get_status() << "\n\n";
//
//    std::cout << "TEST: Enable heater pcdu.set_heater_enable(1):\n";
//    pcdu.set_heater_enable(1);
//    std::cout << "    pcdu.enable_heater()\n";
//    std::cout << "    pcdu.get_status()= " << pcdu.get_status() << "\n\n";
