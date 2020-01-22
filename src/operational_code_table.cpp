#include "../h/operational_code_table.h"
#include "../h/utility.h"
#include "../h/macros.h"
#include "../h/def.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>

std::unordered_map<std::string, std::shared_ptr<Mnemonic>> OperationalCodeTable::_map;

bool OperationalCodeTable::find(std::string operation)
{
    std::smatch s;
    if (std::regex_search(operation, s, Utility::instruction_regex))
    {
        return _map.find(s[1]) != std::end(_map);

    }
    return false;
}

std::shared_ptr<Mnemonic> OperationalCodeTable::get(std::string operation)
{
    std::smatch s;
    if (std::regex_search(operation, s, Utility::instruction_regex))
    {
        return _map.find(s[1])->second;
    }
    return nullptr;
}

uint8_t OperationalCodeTable::getAddressMode(std::string &operation)
{
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
        return REGINDPOM;
    }
    if (std::regex_match(operation, Utility::memdir_regex))
    {
        return MEMDIR;
    }
    if (std::regex_match(operation, Utility::immed_regex_simbol))
    {
        return IMMED_SYM;
    }
    if (std::regex_match(operation, Utility::regindpom_regex_simbol))
    {
        return REGINDPOM_SYM;
    }
    if (std::regex_match(operation, Utility::pc_relative_regex_simbol))
    {
        return PC_RELATIVE_SYM;
    }
    if (std::regex_match(operation, Utility::abs_regex_simbol))
    {
        return ABS_SYM;
    }
    if (std::regex_match(operation, Utility::immed_regex_value))
    {
        return IMMED;
    }

    return 0xf; //ERROR CODE
}

bool OperationalCodeTable::isByteSize(std::string operation)
{
    std::smatch s;
    if (std::regex_search(operation, s, Utility::instruction_regex))
    {
        auto ss = s[2];
        if (ss == "w")
            return false;
        else if (ss == "b")
            return true;
    }
    return false;
}

bool OperationalCodeTable::isLowHigh(std::string operation)
{
    std::smatch s;
    if (std::regex_search(operation, s, Utility::instruction_regex))
    {
        if (s.size() == 4) {
            auto ss = s[3];
            if (ss == "l")
                return false;
            else if (ss == "h")
                return true;
        }
    }
    return false;
}
uint8_t OperationalCodeTable::getAddressModes(std::string operation)
{
    auto mnemonic = OperationalCodeTable::get(operation);
    return mnemonic->getModes();
}

Instruction OperationalCodeTable::constructInstruction(std::vector<std::string> inst)
{
    auto mnemonic = OperationalCodeTable::get(inst[0]);
    return mnemonic->constructInstruction(inst);
}

std::vector<std::string> OperationalCodeTable::getInstruction(std::string operation, std::vector<std::string> line)
{
    auto iterator = std::find(std::begin(line), std::end(line), operation);

    auto instruction = std::vector<std::string>(line.size() - std::distance(std::begin(line), iterator));

    std::copy(iterator, std::end(line), std::begin(instruction));

    return instruction;
}
uint8_t OperationalCodeTable::checkInstruction(std::string operation, std::vector<std::string> line)
{
    std::smatch s;
    if (!std::regex_search(operation, s, Utility::instruction_regex))
    {
        return -1;
    }
    std::string op = s[1];
    auto mneumonic = std::find_if(std::begin(_map), std::end(_map), [&](auto inst) { return inst.first == op; });
    auto instruction = OperationalCodeTable::getInstruction(op, line);
    if(mneumonic->second->getCode() == 0x01 || mneumonic->second->getCode() == 0x018 || mneumonic->second->getCode() == 0x019) 
    {
        return 1;
    }
    if (line.size() < 2)
    {
        PRINT("Los broj argumenata kod operacije: " + line[0]);
        return -1;
    }
    auto address_mode1 = OperationalCodeTable::getAddressMode(line[1]);
    auto address_mode2 = address_mode1;
    auto is_byte_instruction = OperationalCodeTable::isByteSize(operation);

    if (address_mode1 & OperationalCodeTable::getAddressModes(op))
    {
        auto mn = _map.at(op);
        auto code = mn->getCode();
        auto is_single = mn->getIsSingleOperand();
        uint16_t immed_value = 0;

        if (address_mode1 == IMMED)
        {
            immed_value = std::stoi(line[1]);
            return Mnemonic::getInstructionSize(address_mode1, address_mode2, code, is_byte_instruction, immed_value, is_single);
        }
        if (!is_single)
        {
            if (line.size() < 3)
            {
                PRINT("Los broj argumenata kod operacije: " + line[0]);
                return -1;
            }
            address_mode2 = OperationalCodeTable::getAddressMode(line[2]);
            if (address_mode2 == IMMED)
            {
                immed_value = std::stoi(line[2]);
                return Mnemonic::getInstructionSize(address_mode1, address_mode2, code, is_byte_instruction, immed_value, is_single);
            }
        }
        return Mnemonic::getInstructionSize(address_mode1, address_mode2, code, is_byte_instruction, 0, is_single);
    }

    return -1;
}
void OperationalCodeTable::init()
{
    std::string instruction = "halt";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x01, REGDIR, CONTROL_INSTRUCTION, 0, true)));
    instruction = "xchg";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x02, REGDIR, ALU_INSTRUCTION)));
    instruction = "int";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x03, REGDIR, JMP_INSTRUCTION, 0, true)));
    instruction = "mov";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x04, MEMDIR | REGDIR | REGINDPOM | REGINDPOM16 | PC_RELATIVE_SYM, LOAD_STORE_INSTRUCTION)));

    //ALU INSTRUCTIONS
    instruction = "add";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x05, REGDIR, ALU_INSTRUCTION)));
    instruction = "sub";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x06, REGDIR, ALU_INSTRUCTION)));
    instruction = "mul";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x07, REGDIR, ALU_INSTRUCTION)));
    instruction = "div";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x08, REGDIR, ALU_INSTRUCTION)));
    instruction = "cmp";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x09, REGDIR, ALU_INSTRUCTION)));
    instruction = "not";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0A, REGDIR, ALU_INSTRUCTION, 0, true)));
    instruction = "and";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0B, REGDIR, ALU_INSTRUCTION)));
    instruction = "or";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0C, REGDIR, ALU_INSTRUCTION)));
    instruction = "xor";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0D, REGDIR, ALU_INSTRUCTION)));
    instruction = "test";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0E, REGDIR, ALU_INSTRUCTION)));
    instruction = "shl";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x0F, REGDIR, ALU_INSTRUCTION)));
    instruction = "shr";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x10, REGDIR, ALU_INSTRUCTION)));
    //Stack Instructions
    instruction = "push";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x11, REGDIR, STACK_INSTRUCTION, 0, true)));
    instruction = "pop";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x12, REGDIR, STACK_INSTRUCTION, 0, true)));

    //Jump Instructions

    instruction = "jmp";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x13, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "jeq";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x14, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "jne";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x15, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "jgt";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "call";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x17, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "ret";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x18, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    instruction = "iret";
    _map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x19, REGDIR | MEMDIR | REGINDPOM | REGINDPOM16 | REGINDPOM_SYM | PC_RELATIVE_SYM | ABS_SYM, JMP_INSTRUCTION, 0, true)));
    return;
}