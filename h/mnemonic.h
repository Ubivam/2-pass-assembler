#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <unordered_map>
#include "../h/def.h"
#include "../h/macros.h"

class Mnemonic
{
public:
    Mnemonic(std::string &name, uint8_t code, uint8_t modes, uint8_t type, uint8_t _inst_type);
    uint32_t constructInstruction(std::vection<std::string> inst) const;

public:
    bool isValid(uint8_t mode) const;

public:
    static uint8_t getInstructionSize(uint8_t mode);
    static uint8_t getRegisterCode(std::string reg);
    static uint8_t getRegisterIndCode(std::string reg);

private:
    static std::unordered_map<uint8_t, uint8_t> _map;

private:
    std::string _name;
    uint8_t _code;
    uint8_t _modes;
    unit8_t _type;
    uint8_t _inst_type;
};
#endif