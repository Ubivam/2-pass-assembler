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
uint8_t Mnemonic::getInstructionSize(uint8_t addr_mode1, uint8_t addr_mode2, uint8_t code, bool is_byte_size, uint16_t immed_value, bool is_single)
{
	if (code == 0x01 || code == 0x18 || code == 0x19)
	{
		return 1;
	}
	auto first_op = 0;
	auto secound_op = 0;
	switch (addr_mode1)
	{
	case REGDIR:
	case REGIND:
		first_op = 1;
		break;
	case REGINDPOM:
		first_op = 2;
		break;
	case REGINDPOM16:
		first_op = 3;
		break;
	case IMMED:
		if (immed_value <= 255)
		{
			first_op = 2;
		}
		else
		{
			first_op = 3;
		}
		break;
	case MEMDIR:
		first_op = 3;
		break;
	case IMMED_SYM:
		first_op = 3;
		break;
	case REGINDPOM_SYM:
		first_op = 3;
		break;
	case PC_RELATIVE_SYM:
		first_op = 3;
		break;
	case ABS_SYM:
		first_op = 3;
		break;
	}

	if (is_single)
		return 1 + first_op;

	switch (addr_mode2)
	{
	case REGDIR:
	case REGIND:
		secound_op = 1;
		break;
	case REGINDPOM:
		secound_op = 2;
		break;
	case REGINDPOM16:
		secound_op = 3;
		break;
	case IMMED:
		if (immed_value <= 255)
		{
			secound_op = 2;
		}
		else
		{
			secound_op = 3;
		}
		break;
	case MEMDIR:
		secound_op = 3;
		break;
	case IMMED_SYM:
		secound_op = 3;
		break;
	case REGINDPOM_SYM:
		secound_op = 3;
		break;
	case PC_RELATIVE_SYM:
		secound_op = 3;
		break;
	case ABS_SYM:
		secound_op = 3;
		break;
	}
	return 1 + first_op + secound_op;
}
uint8_t Mnemonic::getRegisterCode(std::string reg)
{
	/*auto pos = reg.find_first_of(" +-/*]");
	if (pos != std::string::npos)
	{
		reg = reg.substr(1, pos);
	}*/
	if (reg[0] == 'r')
	{
		auto temp = strtoul(reg.substr(1).c_str(), nullptr, 10);
		if (temp < 0 || temp > 7)
			return 0x11;
		return uint8_t(temp);
	}
	if (reg[0] == 'p')
		return 0x7;

	if (reg[0] == 's')
		return 0x6;

	return 0x11; //Kod Greske
}
uint16_t Mnemonic::getRegisterValue(std::string reg)
{

	auto pos = reg.find_first_of("r[&$*]");
	if (pos != std::string::npos)
	{
		if (reg[pos] != ']' && reg[pos] == 'r' && reg[pos + 2] == '[')
		{
			auto pos = reg.find_first_of("[");
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return uint16_t(strtoul(reg.c_str(), nullptr, 16));
		}
		if (reg[pos] == '&')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return uint16_t(strtoul(reg.c_str(), nullptr, 16));
		}
		if (reg[pos] == '*')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return uint16_t(strtoul(reg.c_str(), nullptr, 16));
		}
		if (reg[pos] == '[' && reg[pos + 1] != 'R')
		{
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return uint16_t(strtoul(reg.c_str(), nullptr, 16));
		}
	}
	if (Utility::isNumber(reg))
	{
		return uint16_t(strtoul(reg.c_str(), nullptr, 16));
	}
	return 0xFFFF;
}
std::string Mnemonic::getRegisterSymbol(std::string reg)
{

	auto pos = reg.find_first_of("r[&$*]");
	if (pos != std::string::npos)
	{
		if (reg[pos] != ']' && reg[pos] == 'r' && reg[pos + 2] == '[')
		{
			auto pos = reg.find_first_of("[");
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return reg;
		}
		if (reg[pos] == '&')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return reg;
		}
		if (reg[pos] == '$')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return reg;
		}
		if (reg[pos] == '*')
		{
			reg = reg.substr(pos + 1, reg.size() - 1);
			return reg;
		}
		if (reg[pos] == '[' && reg[pos + 1] != 'R')
		{
			auto pos2 = reg.find_first_of("]");
			if (pos2 != std::string::npos)
			{
				reg = reg.substr(pos + 1, pos2 - pos - 1);
			}
			return reg;
		}
	}
	return "ERROR";
}
Instruction Mnemonic::constructInstruction(std::vector<std::string> inst) const
{
	Instruction ret;
	std::string symb_str;
	uint8_t currentWord = 0;
	currentWord = _code << 3;
	uint8_t byteCounter = 0;
	bool activateOperands;
	bool isSymbol;
	bool is_byte_size = OperationalCodeTable::isByteSize(inst[0]);
	bool is_high = OperationalCodeTable::isLowHigh(inst[0]);

	if (is_byte_size)
	{
		currentWord &= ~(0x1 << 2);
	}
	else
	{
		currentWord |= 0x1 << 2;
	}
	ret.push_back(currentWord);
	byteCounter++;
	currentWord = 0;
	if (_code == 0x01 || _code == 0x18 || _code == 0x19)
	{
		return ret;
	}
	auto addr_mode = OperationalCodeTable::getAddressMode(inst[byteCounter]);

	switch (addr_mode)
	{
	case IMMED:
		isSymbol = false;
		activateOperands = true;
		currentWord |= IMMED << 5;
		break;
	case REGIND:
		isSymbol = false;
		activateOperands = false;
		currentWord |= REGIND << 5;
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		break;
	case REGDIR:
		isSymbol = false;
		activateOperands = false;
		currentWord |= REGDIR << 5;
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		//TODO: L/H
		break;
	case REGINDPOM:
		isSymbol = false;
		activateOperands = true;
		currentWord |= REGINDPOM16 << 5;
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		break;
	case REGINDPOM16:
		isSymbol = false;
		activateOperands = true;
		currentWord |= REGINDPOM16 << 5;
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		break;
	case MEMDIR:
		isSymbol = false;
		activateOperands = true;
		currentWord |= MEMDIR << 5;
		break;
		//SYMBOL
	case IMMED_SYM:
		isSymbol = true;
		activateOperands = false;
		symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
		currentWord |= IMMED << 5;
		break;
	case REGINDPOM_SYM:
		isSymbol = true;
		activateOperands = false;
		symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
		currentWord |= REGINDPOM16 << 5;
		currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
		break;
	case PC_RELATIVE_SYM:
		isSymbol = true;
		activateOperands = false;
		symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
		currentWord |= 0x7 << 1;
		currentWord |= REGINDPOM16 << 5;
		break;
	case ABS_SYM:
		isSymbol = true;
		activateOperands = false;
		symb_str = inst[byteCounter];
		currentWord |= MEMDIR << 5;
		break;
	}
	currentWord |= is_high ? HIGH : LOW;
	ret.push_back(currentWord);
	byteCounter++;
	currentWord = 0;

	if (activateOperands)
	{
		auto value = Mnemonic::getRegisterValue(inst[byteCounter - 1]);
		if (value > 255)
		{
			uint8_t first = (uint8_t)((value & 0xFF00) >> 8);
			uint8_t second = (uint8_t)(value & 0x00FF);
			ret.push_back(second);
			ret.push_back(first);
		}
		else
		{
			uint8_t val = (uint8_t)(value & 0x00FF);
			ret.push_back(val);
		}
	}
	if (isSymbol)
	{
		bool foundSymb = false;
		auto as = Assembler::getInstace();
		for (auto const &symbol : as->symbolTable)
		{
			if (symbol->getName().compare(symb_str) == 0)
			{
				foundSymb = true;
				std::shared_ptr<RelocationEntry> entry;
				auto isAbsRealocationType = !(addr_mode == REGINDPOM || addr_mode == REGINDPOM16 || addr_mode == PC_RELATIVE_SYM || addr_mode == REGINDPOM_SYM);
				entry = std::make_shared<RelocationEntry>(as->currentSection->getLocationCounter() - 2, isAbsRealocationType, symbol->getIndex());
				if (entry)
				{
					as->currentSection->insertRealocationEntry(entry);
				}
				//Value generation
				uint16_t value;
				if ((symbol->isLocal()) && isAbsRealocationType)
				{
					value = (uint16_t)symbol->getOffset();
				}
				if (!(symbol->isLocal()) && isAbsRealocationType)
				{
					value = 0x0000;
				}
				if (!(symbol->isLocal()) && !isAbsRealocationType)
				{
					value = 0xFFFC;
				}
				if ((symbol->isLocal()) && !isAbsRealocationType)
				{
					value = 0x0002;
				}
				uint8_t first = (uint8_t)((value & 0xFF00) >> 8);
				uint8_t second = (uint8_t)(value & 0x00FF);
				ret.push_back(second);
				ret.push_back(first);
				break;
			}
		}
		if (!foundSymb)
		{
			uint8_t error = (uint8_t)0xFF;
			ret.push_back(error);
			ret.push_back(error);
		}
	}
	if (!_single_operand)
	{
		addr_mode = OperationalCodeTable::getAddressMode(inst[byteCounter]);
		switch (addr_mode)
		{
		case IMMED:
			isSymbol = false;
			activateOperands = true;
			currentWord |= IMMED << 5;
			break;
		case REGIND:
			isSymbol = false;
			activateOperands = false;
			currentWord |= REGIND << 5;
			currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
			break;
		case REGDIR:
			isSymbol = false;
			activateOperands = false;
			currentWord |= REGDIR << 5;
			currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
			//TODO: L/H
			break;
		case REGINDPOM:
			isSymbol = false;
			activateOperands = true;
			currentWord |= REGINDPOM16 << 5;
			currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
			break;
		case REGINDPOM16:
			isSymbol = false;
			activateOperands = true;
			currentWord |= REGINDPOM16 << 5;
			currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
			break;
		case MEMDIR:
			isSymbol = false;
			activateOperands = true;
			currentWord |= MEMDIR << 5;
			break;
			//SYMBOL
		case IMMED_SYM:
			isSymbol = true;
			activateOperands = false;
			symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
			currentWord |= IMMED << 5;
			break;
		case REGINDPOM_SYM:
			isSymbol = true;
			activateOperands = false;
			symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
			currentWord |= REGINDPOM16 << 5;
			currentWord |= Mnemonic::getRegisterCode(inst[byteCounter]) << 1;
			break;
		case PC_RELATIVE_SYM:
			isSymbol = true;
			activateOperands = false;
			symb_str = Mnemonic::getRegisterSymbol(inst[byteCounter]);
			currentWord |= REGINDPOM16 << 5;
			break;
		case ABS_SYM:
			isSymbol = true;
			activateOperands = false;
			symb_str = inst[byteCounter];
			currentWord |= MEMDIR << 5;
			break;
		}
		currentWord |= is_high ? HIGH : LOW;
		ret.push_back(currentWord);
		byteCounter++;
		currentWord = 0;
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
		if (isSymbol)
		{
			bool foundSymb = false;
			auto as = Assembler::getInstace();

			for (auto const &symbol : as->symbolTable)
			{
				if (symbol->getName().compare(symb_str) == 0)
				{
					foundSymb = true;
					std::shared_ptr<RelocationEntry> entry;
					auto isAbsRealocationType = !(addr_mode == REGINDPOM || addr_mode == REGINDPOM16 || addr_mode == PC_RELATIVE_SYM || addr_mode == REGINDPOM_SYM);
					entry = std::make_shared<RelocationEntry>(as->currentSection->getLocationCounter() -2 , isAbsRealocationType, symbol->getIndex());
					if (entry)
					{
						as->currentSection->insertRealocationEntry(entry);
					}
					//Value generation
					uint16_t value;
					if ((symbol->isLocal()) && isAbsRealocationType)
					{
						value = (uint16_t)symbol->getOffset();
					}
					if (!(symbol->isLocal()) && isAbsRealocationType)
					{
						value = 0x0000;
					}
					if (!(symbol->isLocal()) && !isAbsRealocationType)
					{
						value = 0xFFFC;
					}
					if ((symbol->isLocal()) && !isAbsRealocationType)
					{
						value = 0x0002;
					}
					uint8_t first = (uint8_t)((value & 0xFF00) >> 8);
					uint8_t second = (uint8_t)(value & 0x00FF);
					byteCounter += 2;
					ret.push_back(second);
					ret.push_back(first);
					break;
				}
			}
			if (!foundSymb)
			{
				byteCounter += 2;
				uint8_t error = (uint8_t)0xFF;
				ret.push_back(error);
				ret.push_back(error);
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