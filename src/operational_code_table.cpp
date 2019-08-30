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
    
}
uint32_t OperationalCodeTable::constructSecountWord(std::vector<std::string> inst, std::shared_ptr<Section> currentSection)
{
	auto op = inst[inst.size() - 1];
	uint32_t ret = 0;
	auto dolar = op[0] == '$';
	if (dolar)op = op.substr(1);
	std::shared_ptr<RelocationEntry> entry = std::shared_ptr<RelocationEntry>(nullptr);
	switch (OperationalCodeTable::getAddressMode(inst))
	{

	}
	return ret;
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
	std::string instruction = "HALT";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x1, REGDIR, JMP_INSTRUCTION)));
	instruction = "XCHD";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x2, REGDIR, ALU_INSTRUCTION)));
	instruction = "INT";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x3, REGDIR, JMP_INSTRUCTION)));
	instruction = "MOV";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x4, MEMDIR | REGDIR | REGINDPOM8 | REGINDPOM16, LOAD_STORE_INSTRUCTION)));
	
	//ALU INSTRUCTIONS
	instruction = "ADD";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x5, REGDIR, ALU_INSTRUCTION)));
	instruction = "SUB";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x6, REGDIR, ALU_INSTRUCTION)));
	instruction = "MUL";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x7, REGDIR, ALU_INSTRUCTION)));
	instruction = "DIV";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x8, REGDIR, ALU_INSTRUCTION)));
	instruction = "CMP";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x9, REGDIR, ALU_INSTRUCTION)));
	instruction = "NOT";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xA, REGDIR, ALU_INSTRUCTION)));
	instruction = "AND";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xB, REGDIR, ALU_INSTRUCTION)));
	instruction = "OR";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xC, REGDIR, ALU_INSTRUCTION)));
	instruction = "XOR";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xD, REGDIR, ALU_INSTRUCTION)));
	instruction = "TEST";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xE, REGDIR, ALU_INSTRUCTION)));
	instruction = "SHL";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0xF, REGDIR, ALU_INSTRUCTION)));
	instruction = "SHR";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x10, REGDIR, ALU_INSTRUCTION)));
	//Stack Instructions
	instruction = "PUSH";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x11, REGDIR, STACK_INSTRUCTION)));
	instruction = "POP";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x12, REGDIR, STACK_INSTRUCTION)));

	//Jump Instructions 

	instruction = "JMP";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x13, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "JEQ";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x14, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "JNE";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x15, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "JGT";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "JMP";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x16, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "CALL";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x17, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "RET";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x18, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	instruction = "IRET";
	_map.insert(std::make_pair<>(instruction, std::make_shared<Mnemonic>(instruction, 0x19, REGDIR | MEMDIR | REGINDPOM8 | REGINDPOM16, JMP_INSTRUCTION)));
	return;
}