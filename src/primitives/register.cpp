#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "register.hpp"

namespace {
std::string to_hex(uint32_t val)
{
    std::ostringstream ss;
    ss << std::setw(8)
       << std::setfill('0')
       << std::hex
       << std::uppercase
       << val;
    return ss.str();
}
} // end namespace

Register::Register(const std::string& name, uint32_t address,
         Access access, uint32_t default_val)
:
name_(name),
address_(address),
access_(access),
default_val_(default_val),
value_(default_val)
{}

const std::string& Register::name() const {
    return name_;
}

uint32_t Register::address() const {
    return address_;
}

Access Register::access() const {
    return access_;
}

uint32_t Register::default_val() const {
     return default_val_;
}

uint32_t Register::read() const
{
    if (access_ == WRITE_ONLY)
    {
        throw std::runtime_error(
            "Register '" + name_ + "' at address '0x" + to_hex(address_) + "' is write-only"
        );
    }
    return value_;
}

void Register::write(uint32_t value)
{
    if (access_ == READ_ONLY)
    {
        throw std::runtime_error(
            "Register '" + name_ + "' at address '0x" + to_hex(address_) + "' is read-only"
        );
    }
    value_ = value;
}

void Register::reset()
{
    value_ = default_val_;
}

// Ignores register access to simulate write from hardware
void Register::sim_write_from_hardware(uint32_t value)
{
    value_ = value;
}

// Ignores register access to simulate read to hardware
uint32_t Register::sim_read_to_hardware() const
{
    return value_;
}


// TESTING STUFF
//int main()
//{
//    std::cout << "Read-Only Register Testing\n";
//    std::cout << "------------------------------------------------------------------------------\n";
//    std::cout << "Instantiate ro_reg:\n";
//    uint32_t ro_addr = 0x00000001;
//    uint32_t ro_read_val;
//    uint32_t ro_write_val = 1;
//    Register ro_reg("ro_reg", ro_addr, READ_ONLY, 0);
//    std::cout << "ro_reg.name()= " << ro_reg.name() << "\n";
//    std::cout << "ro_reg.address()= 0x" << to_hex(ro_reg.address()) << "\n";
//    std::cout << "ro_reg.access()= " << ro_reg.access() << "\n";
//    std::cout << "ro_reg.default_val()= " << ro_reg.default_val() << "\n\n";
//
//    std::cout << "Read from ro_reg:\n";
//    ro_read_val = ro_reg.read();
//    std::cout << "ro_reg.read()= " << ro_read_val << "\n\n";
//
//    std::cout << "Attempt to write to ro_reg:\n";
//    try
//    {
//       ro_reg.write(ro_write_val);
//    }
//    catch (const std::runtime_error& e)
//    {
//        std::cout << "ERROR: " << e.what() << "\n\n";
//    }
//
//    std::cout << "Write-Only Regsiter Testing\n";
//    std::cout << "------------------------------------------------------------------------------\n";
//    uint32_t wo_addr = 0x00000002;
//    uint32_t wo_write_val = 2;
//    std::cout << "Instantiate wo_reg:\n";
//    Register wo_reg("wo_reg", wo_addr, WRITE_ONLY, 0);
//    std::cout << "wo_reg.name()= " << wo_reg.name() << "\n";
//    std::cout << "wo_reg.address()= 0x" << to_hex(wo_reg.address()) << "\n";
//    std::cout << "wo_reg.access()= " << wo_reg.access() << "\n";
//    std::cout << "wo_reg.default_val()= " << wo_reg.default_val() << "\n\n";
//
//    std::cout << "Write to wo_reg: '" << wo_write_val << "'\n";
//    wo_reg.write(wo_write_val);
//    // Used for Debug-Only
//    //std::cout << "value = " << wo_reg.wo_dbg_value() << "\n";
//
//    std::cout << "Attempt to read from wo_reg:\n";
//    try
//    {
//       wo_reg.read();
//    }
//    catch (const std::runtime_error& e)
//    {
//        std::cout << "ERROR: " << e.what() << std::endl << "\n";
//    }
//
//    std::cout << "Read-Write Regsiter Testing\n";
//    std::cout << "------------------------------------------------------------------------------\n";
//    uint32_t rw_addr = 0x00000003;
//    uint32_t rw_write_val = 3;
//    uint32_t rw_read_val;
//    std::cout << "Instantiate rw_reg:\n";
//    Register rw_reg("rw_reg", rw_addr, READ_WRITE, 0);
//    std::cout << "rw_reg.name()= " << rw_reg.name() << "\n";
//    std::cout << "rw_reg.address()= 0x" << to_hex(rw_reg.address()) << "\n";
//    std::cout << "rw_reg.access()= " << rw_reg.access() << "\n";
//    std::cout << "rw_reg.default_val()= " << rw_reg.default_val() << std::endl << "\n";
//
//    std::cout << "Write to rw_reg: '" << rw_write_val << "'\n";
//    rw_reg.write(rw_write_val);
//
//    std::cout << "Read from rw_reg:\n";
//    rw_read_val = rw_reg.read();
//    std::cout << "rw_reg.read()= " << rw_read_val << "\n\n";
//}
