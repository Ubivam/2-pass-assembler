#ifndef MNEMONIC_H
#define MNEMONIC_H

#include <unordered_map>
#include "../h/def.h"
#include "../h/macros.h"

class Mnemonic
{
public:
	Mnemonic(std::string& name, uint8_t code, uint8_t modes, uint8_t type, uint8_t _inst_type = 0, bool singleOperand = false);
	uint8_t constructInstruction(std::vector<std::string> inst) const;

public:
	bool isValid(uint8_t mode) const;

public:
	static uint8_t getInstructionSize(uint8_t mode, bool isByteSize);
	static uint8_t getRegisterCode(std::string reg);
	static uint8_t getRegisterIndCode(std::string reg);

public:
	uint8_t getCode() const;
	std::string getName() const;
	uint8_t getModes() const;
	uint8_t getTypes() const;
	uint8_t getInstType() const;
private:
	static std::unordered_map<uint8_t, uint8_t> _map;

private:
	std::string _name;
	uint8_t _code;
	uint8_t _modes;
	uint8_t _type;
	uint8_t _inst_type;
	bool _single_operand;
};
#endif