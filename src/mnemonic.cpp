#include "../h/mnemonic.h"
#include "../h/operational_code_table.h"

Mnemonic::Mnemonic(std::string &name, uint8_t code, uint8_t mode, uint8_t type, uint8_t inst_type, bool single_operand)
	: _name(name),
	  _code(code),
	  _modes(mode),
	  _type(type),
	  _inst_type(inst_type),
	  _single_operand(single_operand)
{
}
uint8_t Mnemonic::getInstructionSize(uint8_t mode, uint8_t code, bool is_byte_size, bool single_operand)
{
	if (code == 0x01 || code == 0x18 || code == 0x19)
	{
		return 1;
	}
	switch (mode)
	{
	case REGDIR:
	case REGIND:
		return 2;
	case REGINDPOM8:
		if (single_operand)
		{
			return 3;
		}
		else
		{
			return 5;
		}
	case REGINDPOM16:
		if (single_operand)
		{
			return 5;
		}
		else
		{
			return 7;
		}

	case IMMED:
	case MEMDIR:
		if (single_operand)
		{
			if (is_byte_size)
			{
				return 3;
			}
			else
			{
				return 4;
			}
		}
		else
		{
			if (is_byte_size)
			{
				return 5;
			}
			else
			{
				return 7;
			}
		}
	}
	return 0;
}
uint8_t Mnemonic::getRegisterCode(std::string reg)
{
	auto pos = reg.find_first_of(" +-/*]");
	if (pos != std::string::npos)
	{
		reg = reg.substr(1, pos);
	}
	if (reg[0] == 'R')
	{
		return uint8_t(strtoul(reg.substr(1).c_str(), nullptr, 10));
	}
	if (reg[0] == 'P')
		return 0xF;

	return 0x11; //Kod Greske
}
uint16_t Mnemonic::getRegisterValue(std::string reg)
{

	auto pos = reg.find_first_of(" [#+-/*]");
	if (pos != std::string::npos)
	{
		if (reg[pos] != ']' && reg[pos] == '[' && reg[pos + 1] == 'R')
		{
			auto pos = reg.find_first_of(" +-/*]");
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return uint16_t(strtoul(reg.c_str(), nullptr, 10));
		}
		if (reg[pos] == '#')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return uint16_t(strtoul(reg.c_str(), nullptr, 10));
		}
		if (reg[pos] == '[' && reg[pos + 1] != 'R')
		{
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return uint16_t(strtoul(reg.c_str(), nullptr, 10));
		}
	}
	return 0xFFFF; //kod greske
}
Instruction Mnemonic::constructInstruction(std::vector<std::string> inst) const
{
	Instruction ret;
	uint8_t currentWord = 0;
	currentWord = _code << 3;
	uint8_t byteCounter = 0;
	bool activateOperands;
	bool is_byte_size = OperationalCodeTable::isByteSize(inst);
	uint8_t instruction_size = Mnemonic::getInstructionSize(_modes, _code, is_byte_size, _single_operand);
	if (is_byte_size)
	{
		currentWord &= 0x0 << 2;
	}
	else
	{
		currentWord &= 0x1 << 2;
	}
	ret.push_back(currentWord);
	byteCounter++;
	currentWord = 0;
	if (instruction_size == 1)
		return ret;

	auto addr_mode = OperationalCodeTable::getAddressMode(inst[byteCounter]);

	switch (addr_mode)
	{
	case IMMED:
		activateOperands = true;
		currentWord |= IMMED << 5;
		break;
	case REGIND:
		activateOperands = false;
		currentWord |= REGIND << 5;
		break;
	case REGDIR:
		activateOperands = false;
		currentWord |= REGDIR << 5;
		//TODO: L/H
		break;
	case REGINDPOM8:
		activateOperands = true;
		currentWord |= REGINDPOM8 << 5;
		break;
	case REGINDPOM16:
		activateOperands = true;
		currentWord |= REGINDPOM16 << 5;
		break;
	case MEMDIR:
		activateOperands = true;
		currentWord |= MEMDIR << 5;
		break;
	}
	currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
	ret.push_back(currentWord);
	byteCounter++;
	currentWord = 0;

	switch (_type)
	{
	case ALU_INSTRUCTION:
		break;
	case CONTROL_INSTRUCTION:
		return ret;
	case JMP_INSTRUCTION:
		if (_code == 0x18 || _code == 0x19)
			return ret;

	case STACK_INSTRUCTION:
		break;

	case LOAD_STORE_INSTRUCTION:
		break;
	}
	if (activateOperands)
	{
		auto value = Mnemonic::getRegisterValue(inst[byteCounter - 1]);
		if (value > 255)
		{
			uint8_t first = (uint8_t)((value & 0xFF00) >> 8);
			uint8_t second = (uint8_t)(value & 0x00FF);
			byteCounter += 2;
			ret.push_back(second);
			ret.push_back(first);
		}
		else
		{
			uint8_t val = (uint8_t)(value & 0x00FF);
			ret.push_back(val);
			byteCounter++;
		}
	}
	if (!_single_operand)
	{
		addr_mode = OperationalCodeTable::getAddressMode(inst[byteCounter]);
		switch (addr_mode)
		{
		case IMMED:
			activateOperands = true;
			currentWord |= IMMED << 5;
			break;
		case REGIND:
			activateOperands = false;
			currentWord |= REGIND << 5;
			break;
		case REGDIR:
			activateOperands = false;
			currentWord |= REGDIR << 5;
			//TODO: L/H
			break;
		case REGINDPOM8:
			activateOperands = true;
			currentWord |= REGINDPOM8 << 5;
			break;
		case REGINDPOM16:
			activateOperands = true;
			currentWord |= REGINDPOM16 << 5;
			break;
		case MEMDIR:
			activateOperands = true;
			currentWord |= MEMDIR << 5;
			break;
		}
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		ret.push_back(currentWord);
		byteCounter++;
		currentWord = 0;
		switch (_type)
		{
		case ALU_INSTRUCTION:
			break;
		case CONTROL_INSTRUCTION:
			return ret;
		case JMP_INSTRUCTION:
			if (_code == 0x18 || _code == 0x19)
				return ret;

		case STACK_INSTRUCTION:
			break;

		case LOAD_STORE_INSTRUCTION:
			break;
		}
		if (activateOperands)
		{
			auto value = Mnemonic::getRegisterValue(inst[byteCounter - 1]);
			if (value > 255)
			{
				uint8_t first = (uint8_t)((value & 0xFF00) >> 8);
				uint8_t second = (uint8_t)(value & 0x00FF);
				byteCounter += 2;
				ret.push_back(second);
				ret.push_back(first);
			}
			else
			{
				uint8_t val = (uint8_t)(value & 0x00FF);
				ret.push_back(val);
				byteCounter++;
			}
		}
	}
	return ret;
}
uint8_t Mnemonic::getCode() const
{
	return _code;
}
std::string Mnemonic::getName() const
{
	return _name;
}
uint8_t Mnemonic::getModes() const
{
	return _modes;
}
uint8_t Mnemonic::getTypes() const
{
	return _type;
}
uint8_t Mnemonic::getInstType() const
{
	return _inst_type;
}
bool Mnemonic::getIsSingleOperand() const
{
	return _single_operand;
}