#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "component.hpp"

namespace
{
std::string to_hex(uint32_t value)
{
    std::ostringstream stream;
    stream << std::setw(8)
           << std::setfill('0')
           << std::hex
           << std::uppercase
           << value;
    return stream.str();
}
} //end namespace

Component::Component(const std::string& name)
:
name_(name)
{}

const std::string& Component::name() const {
    return name_;
}

uint32_t Component::read_reg(uint32_t address) const
{
    auto it = registers_.find(address);
    if (it == registers_.end())
    {
        throw std::runtime_error(
            "Register at address '0x" + to_hex(address) + "' does not exist\n"
        );
    }
    return it->second.read();
}

void Component::validate_write_reg(uint32_t address, uint32_t value) const
{
}

void Component::write_reg(uint32_t address, uint32_t value)
{
    auto it = registers_.find(address);
    if (it == registers_.end())
    {
        throw std::runtime_error(
            "Register at address '0x" + to_hex(address) + "' does not exist\n"
        );
    }
    validate_write_reg(address, value);
    it->second.write(value);
}

void Component::reset_reg(uint32_t address)
{
    auto it = registers_.find(address);
    if (it == registers_.end())
    {
        throw std::runtime_error(
            "Register at address '0x" + to_hex(address) + "' does not exist\n"
        );
    }
    it->second.reset();
}

void Component::add_reg(const std::string& name, uint32_t address,
         Access access, uint32_t default_val)
{
    if (registers_.find(address) != registers_.end())
    {
        throw std::runtime_error(
            "Register already exists at address: '0x" + to_hex(address) + "'\n"
        );
    }
    registers_.insert({address, Register(name, address, access, default_val)});
}

// Ignores register access to simulate write from hardware
void Component::sim_wreg_from_hardware(uint32_t address, uint32_t value)
{
    auto it = registers_.find(address);
    if (it == registers_.end())
    {
        throw std::runtime_error(
            "Register at address '0x" + to_hex(address) + "' does not exist\n"
        );
    }
    validate_write_reg(address, value);
    it->second.sim_write_from_hardware(value);
}

// Ignores register access to simulate read to hardware
uint32_t Component::sim_rreg_to_hardware(uint32_t address) const
{
    auto it = registers_.find(address);
    if (it == registers_.end())
    {
        throw std::runtime_error(
            "Register at address '0x" + to_hex(address) + "' does not exist\n"
        );
    }
    return it->second.sim_read_to_hardware();
}


//std::cout << "Test Component\n";
//std::cout << "------------------------------------------------------------------------------\n";
//std::cout << "Test Component instantiation\n";
//Component comp("comp1");
//uint32_t reg1_addr = 0x00000001;
//std::cout << "Component name= '" << comp.name() << "'\n\n";
//
//// Test Component add_reg()
//comp.add_reg("reg1", 0x00000001, READ_WRITE, 0);
//
//std::cout << "Test Component add_reg() for register that already exists\n";
//try
//{
//    comp.add_reg("reg1", 0x00000001, READ_WRITE, 0);
//}
//catch (const std::runtime_error& e)
//{
//    std::cout << "ERROR: " << e.what() << "\n";
//}
//
//std::cout << "Test Component read_reg()\n";
//std::cout << "comp.read_reg(0x" << to_hex(reg1_addr) << ").read= "
//          <<  comp.read_reg(reg1_addr) << "\n\n";
//
//std::cout << "Test Component read_reg(address) for reg(address) that doesn't exist\n";
//try
//{
//    comp.read_reg(0x12345678);
//}
//catch (const std::runtime_error& e)
//{
//    std::cout << "ERROR: " << e.what() << "\n";
//}
//
//std::cout << "Test Component write_reg(address, value)\n";
//comp.write_reg(reg1_addr, 0xDEADBEEF);
//std::cout << "comp.read_reg(0x" << to_hex(reg1_addr) << ").read= "
//          <<  "0x" << to_hex(comp.read_reg(reg1_addr)) << "\n\n";
//
//std::cout << "Test Component write_reg(address) for reg(address) that doesn't exist\n";
//try
//{
//    comp.write_reg(0x12345678, 0xDEADBEEF);
//}
//catch (const std::runtime_error& e)
//{
//    std::cout << "ERROR: " << e.what() << "\n";
//}
