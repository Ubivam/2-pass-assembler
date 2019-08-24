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
std::vector<std::string> OperationalCodeTable::getInstruction(std::string operation, std::vector<std::string> line)
{
    auto iterator = std::find(std::begin(line), std::end(line), operation);

    auto instruction = std::vector<std::string> (line.size()- std::distance(std::begin(line),iterator));

    std::copy(iterator, std::end(line), std::begin(instruction));

    return instruction;
}
uint8_t OperationalCodeTable::checkInstruction(std::string op,std::vector<std::string> line)
{
	auto mneumonic = std::find_if(std::begin(_map), std::end(_map), [&](auto inst) {return inst.first == op; });

	auto instruction = OperationalCodeTable::getInstruction(op, line);

	auto address_mode = OperationalCodeTable::getAddressMode(instruction);

	if(address_mode & OperationalCodeTable::getAddressModes(op))
	{
		return Mnemonic::getInstructionSize(address_mode);
	}

	return -1;
}
void OperationalCodeTable::init()
{
    return;
}