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
uint8_t Mnemonic::getInstructionSize(uint8_t mode, bool isByteSize)
{
	if (_code == 0x01 || _code == 0x18 || _code == 0x19)
	{
		return 1;
	}
	switch (mode)
	{
	case REGDIR:
	case REGIND:
		return 2;
	case REGINDPOM8:
		if (_single_operand)
		{
			return 3;
		}
		else
		{
			return 5;
		}
	case REGINDPOM16:
		if (_single_operand)
		{
			return 5;
		}
		else
		{
			return 7;
		}

	case IMMED:
	case MEMDIR:
		if (_single_operand)
		{
			if (isByteSize)
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
			if (isByteSize)
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
	return uint8_t();
}
Instruction Mnemonic::constructInstruction(std::vector<std::string> inst) const
{
	Instruction ret;
	uint8_t currentWord = 0;
	_currentWord = _code << 3;
	bool is_byte_size = OperationalCodeTable::isByteSize(inst);
	uint8_t instruction_size = Mnemonic::getInstructionSize(_modes, is_byte_size);
	if (is_byte_size)
	{
		currentWord &= 0x0 << 2;
	}
	else
	{
		currentWord &= 0x1 << 2;
	}
	ret.push_back(currentWord);
	currentWord = 0;
	if (instruction_size == 1)
		return ret;

	auto addr_mode = OperationalCodeTable::getAddressMode(inst);

	switch (addr_mode)
	{
	case:
		IMMED
		word |= IMMED << 5;
		break;
	case:
		REGIND
		word |= REGIND << 5;
		break;
	case:
		REGDIR
		word |= REGDIR << 5;
		break;
	case:
		REGINDPOM8
		word |= REGINDPOM8 << 5;
		break;
	case:
		REGINDPOM16
		word |= REGINDPOM16 << 5;
		break;
	case:
		MEMDIR
		word |= MEMDIR << 5;
		break;
	default:
		break;
	}
	return nullptr;
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