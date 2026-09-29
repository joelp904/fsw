#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "register.hpp"

class Component
{
public:
    explicit Component(const std::string& name);

    const std::string& name() const;

protected:

    uint32_t read_reg(uint32_t address) const;

    void write_reg(uint32_t address, uint32_t value);

    void reset_reg(uint32_t address);

    void add_reg(const std::string& name, uint32_t address,
                 Access access, uint32_t default_val = 0);

    virtual void validate_write_reg(uint32_t address, uint32_t value) const;

    // Ignores register access to simulate write from hardware
    void sim_wreg_from_hardware(uint32_t address, uint32_t value);

    // Ignores register access to simulate read to hardware
    uint32_t sim_rreg_to_hardware(uint32_t address) const;

private:
    std::string name_;
    std::unordered_map<uint32_t, Register> registers_;
};
