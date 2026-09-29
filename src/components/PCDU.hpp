#pragma once

#include "../primitives/component.hpp"

enum Heater_num
{
    HEATER0,
    HEATER1
};

class PCDU : public Component 
{
public:
    PCDU();

    std::string get_status(Heater_num heater) const;
    void set_heater_enable(Heater_num heater, bool enable);

private:
    bool heater0_is_powered_ = false;
    bool heater1_is_powered_ = false;
    static constexpr uint32_t HEATER0_PWR_ADDR = 0x00000000;
    static constexpr uint32_t HEATER1_PWR_ADDR = 0x00000004;

    // Simulation Only
    friend class Thermal_sim;
    bool sim_heater_powered(Heater_num heater) const;
};
