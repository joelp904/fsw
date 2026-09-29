#pragma once

#include <cstdint>
#include <string>

enum Access
{
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
};

class Register
{
public:
    Register(const std::string& name, uint32_t address,
             Access access, uint32_t default_val = 0);

    const std::string& name() const;

    uint32_t address() const;

    Access access() const;

    uint32_t default_val() const;

    uint32_t read() const;

    void write(uint32_t value);

    void reset();

    // Ignores register access to simulate write from hardware
    void sim_write_from_hardware(uint32_t value);

    // Ignores register access to simulate read to hardware
    uint32_t sim_read_to_hardware() const;

private:
    // Member Variables
    std::string name_;
    const uint32_t address_;
    const Access access_;
    const uint32_t default_val_;
    uint32_t value_;
};
