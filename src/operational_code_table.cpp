#include "../h/operational_code_table.h"
#include "../h/utility.h"
#include "../h/macros.h"
#include "../h/def.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

std::unordered_map<std::string, std::shared_ptr<Mnemonic>> OperationalCodeTable::_map;

bool OperationalCodeTable::find(std::string operand)
{
    return _map.find(operand) != std::end(_map);
}

std::shared_ptr<Mnemonic> OperationalCodeTable::get(std::string operand)
{
    return _map.find(operand)->second;
}

uint8_t OperationalCodeTable::getAddressMode(std::vector<std::string> &instruction)
{
    auto operation = instruction[instruction.size() - 1];
    if (std::regex_match(operation, Utility::register_regex))
    {
        return REGDIR;
    }
    if (std::regex_match(operation, Utility::regind_regex))
    {
        return REGIND;
    }
    if (std::regex_match(operation, Utility::regindpom_regex))
    {
        return REGINDPOM16;
    }
    if (std::regex_match(operation, Utility::immed_regex))
    {
        return IMMED;
    }
    if (std::regex_match(operation, Utility::memdir_regex))
    {
        return MEMDIR;
    }

    return 0xf; //ERROR CODE
}

bool OperationalCodeTable::isByteSize(std::vector<std::string> &instruction)
{
    std::smatch s;
    auto operation = instruction[instruction.size()-1);
    if(std::regex_search(operation,s, Utility::instruction_regex))
    {
        auto ss = s[2];
        if(ss="w") return false;
        else if(ss="b") return true;
    }
    return false;
}

uint8_t OperationalCodeTable::getAddressModes(std::string operation)
{
    auto mnemonic = OperationalCodeTable::get(operation);
    return mnemonic->getModes();
}

uint32_t OperationalCodeTable::constructInstruction(std::vector<std::string> inst)
{
    auto mnemonic = OperationalCodeTable::get(inst[0]);
    return mnemonic->constructInstruction(inst);
}
uint32_t OperationalCodeTable::constructSecountWord(std::vector<std::string> inst, std::shared_ptr<Section> currentSection)
{
    auto op = inst[inst.size() - 1];
    uint32_t ret = 0;
    auto dolar = op[0] == '$';
    if (dolar)
        op = op.substr(1);
    std::shared_ptr<RelocationEntry> entry = std::shared_ptr<RelocationEntry>(nullptr);
    switch (OperationalCodeTable::getAddressMode(inst))
    {
    }
    return ret;
}
std::vector<std::string> OperationalCodeTable::getInstruction(std::string operation, std::vector<std::string> line)
{
    auto iterator = std::find(std::begin(line), std::end(line), operation);

    auto instruction = std::vector<std::string>(line.size() - std::distance(std::begin(line), iterator));

    std::copy(iterator, std::end(line), std::begin(instruction));

    return instruction;
}
uint8_t OperationalCodeTable::checkInstruction(std::string op, std::vector<std::string> line)
{
    auto mneumonic = std::find_if(std::begin(_map), std::end(_map), [&](auto inst) { return inst.first == op; });

    auto instruction = OperationalCodeTable::getInstruction(op, line);

    auto address_mode = OperationalCodeTable::getAddressMode(instruction);

    auto isByteInstruction = OperationalCodeTable::isByteSize(instruction);

    if (address_mode & OperationalCodeTable::getAddressModes(op))
    {
        return Mnemonic::getInstructionSize(address_mode, isByteInstruction);
    }

    return -1;
}
void OperationalCodeTable::init()
{
    std::string instruction = "HALT";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x01, REGDIR, JMP_INSTRUCTION, 0, true)));
    instruction = "XCHD";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x02, REGDIR, ALU_INSTRUCTION)));
    instruction = "INT";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x03, REGDIR, JMP_INSTRUCTION, 0, true)));
    instruction = "MOV";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x04, MEMDIR | REGDIR | REGINDPOM8 | REGINDPOM16, LOAD_STORE_INSTRUCTION)));

    //ALU INSTRUCTIONS
    instruction = "ADD";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x05, REGDIR, ALU_INSTRUCTION)));
    instruction = "SUB";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x06, REGDIR, ALU_INSTRUCTION)));
    instruction = "MUL";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x07, REGDIR, ALU_INSTRUCTION)));
    instruction = "DIV";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x08, REGDIR, ALU_INSTRUCTION)));
    instruction = "CMP";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x09, REGDIR, ALU_INSTRUCTION)));
    instruction = "NOT";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0A, REGDIR, ALU_INSTRUCTION, 0, true)));
    instruction = "AND";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0B, REGDIR, ALU_INSTRUCTION)));
    instruction = "OR";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0C, REGDIR, ALU_INSTRUCTION)));
    instruction = "XOR";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0D, REGDIR, ALU_INSTRUCTION)));
    instruction = "TEST";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0E, REGDIR, ALU_INSTRUCTION)));
    instruction = "SHL";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0F, REGDIR, ALU_INSTRUCTION)));
    instruction = "SHR";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x10, REGDIR, ALU_INSTRUCTION)));
    //Stack Instructions
    instruction = "PUSH";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x11, REGDIR, STACK_INSTRUCTION, 0, true)));
    instruction = "POP";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x12, REGDIR, STACK_INSTRUCTION, 0, true)));

    //Jump Instructions

    instruction = "JMP";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x13, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "JEQ";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x14, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "JNE";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x15, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "JGT";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "JMP";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "CALL";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x17, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "RET";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x18, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    instruction = "IRET";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x19, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION, 0, true)));
    return;
}