#include "../h/mnemonic.h"

Mnemonic::Mnemonic(std::string &name, uint8_t code,uint8_t mode, uint8_t type, uint8_t inst_type)
: _name(name),
  _code(code),
  _modes(mode),
  _type(type),
  _inst_type(inst_type)
{
}
uint8_t Mnemonic::getInstructionSize(uint8_t mode)
{
    switch (mode)
	{
	case REGDIR:
	case REGIND:
		return 4;
	case IMMED:
	case MEMDIR:
	case REGINDPOM16:
    case REGINDPOM8:
		return 8;
	}
	return 0;
}
uint8_t Mnemonic::getRegisterCode(std::string reg)
{
	return uint8_t();
}
uint8_t Mnemonic::constructInstruction(std::vector<std::string> inst) const
{
	uint32_t ret = _code << 24;
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