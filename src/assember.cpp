#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/utility.h"
#include "../h/section.h"
#include "../h/operational_code_table.h"

SymbolTable Assembler::symbolTable;
SectionTable Assembler::sectionTable;
Assembler::Assembler()
{
}

bool Assembler::firstPass(ArrayOfStrings &instructions)
{
	auto _currentSection = std::shared_ptr<Section>(nullptr);
	uint16_t value = 0;

	for (auto &line : instructions)
	{
		uint8_t inst = 0;
		for (auto &word : line)
		{
			if (word == ".end")
			{
				return true;
			}
			if (word == ".global")
			{
				auto it = &word;
				while (it != &line.back())
				{
					auto symbol = std::make_shared<Symbol>(*(it + 1), _currentSection, 0, NOT_SECTION, GLOBAL_SYMBOL);
					symbol->setIndex(uint16_t(symbolTable.size()) + 1);
					symbolTable.insert(symbol);
					it++;
				}
				continue;
			}
			if (word == ".extern")
			{
				auto it = &word;
				while (it != &line.back())
				{
					auto symbol = std::make_shared<Symbol>(*(it + 1), _currentSection, 0, NOT_SECTION, GLOBAL_SYMBOL);
					symbol->setIndex(uint16_t(symbolTable.size()) + 1);
					symbolTable.insert(symbol);
					it++;
				}
				continue;
			}
			if (word == ".equ")
			{
				auto symbolName = &word + 1;
				auto value = &word + 2;
				auto symbol = std::make_shared<Symbol>(word, _currentSection, _currentSection->getBeginLocationCounter(), NOT_SECTION, GLOBAL_SYMBOL);
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);
				continue;
			}
			if (word == ".byte")
			{
				auto it = &word;
				while (it != &line.back())
				{
					_currentSection->incLocationCounter(1);
					auto symbol = std::make_shared<Symbol>(word, _currentSection, _currentSection->getLocationCounter());
					symbol->setIndex(uint16_t(symbolTable.size()) + 1);
					symbolTable.insert(symbol);
					it++;
				}
				continue;
			}
			if (word == ".align")
			{
				if (!_currentSection)
				{
					PRINT("Current section is nullptr");
					return false;
				}
				uint8_t i = 0;
				while ((std::stoi((*(&word + i))) % _currentSection->getLocationCounter()) == 0)
				{
					_currentSection->incLocationCounter(1);
					i++;
				}
				continue;
			}
			if (word == ".skip")
			{
				if (_currentSection != nullptr)
					_currentSection->incLocationCounter(stoi(*(&word + 1)));
				continue;
			}
			if (word.find(":") != std::string::npos)
			{
				//if(inst & INSTRUCTION)
				if (!_currentSection)
				{
					PRINT("Current section is nullptr");
					return false;
				}
				auto label = word.substr(0, word.length() - 1);
				auto symbol = std::make_shared<Symbol>(label, _currentSection, _currentSection->getLocationCounter() - _currentSection->getBeginLocationCounter());
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);
				symbolTable.insert(symbol);
				continue;
			}
			// if it is a section
			if (word[0] == '.')
			{
				auto name = word;
				//  auto section = std::make_shared<Section>(new Section(name, _symbolTable->size() + 1, value));
				PRINT(symbolTable.size());
				auto section = std::make_shared<Section>(name, symbolTable.size() + 1, value);
				value = 0;

				sectionTable.insert(section);
				_currentSection = section;

				auto symbol = std::make_shared<Symbol>(name, _currentSection, _currentSection->getLocationCounter(), IS_SECTION, true);
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);
				symbolTable.insert(symbol);
				continue;
			}
			//Is it instruction
			if (OperationalCodeTable::find(word))
			{
				auto bytes = OperationalCodeTable::checkInstruction(word, line);
				if (word == "RET")
					bytes = 4;
				if (bytes == -1)
				{
					PRINT("Error ocured!");
					return false;
				}
				_currentSection->incLocationCounter(bytes);
				continue;
			}
			//PRINT("You have Syntax error");
			//return false;
		}
	}
	return true;
}

bool Assembler::secoundPass(ArrayOfStrings &instructions)
{
	for (auto &sec : sectionTable)
	{
		//TODO: Save and reset loc counter
	}
	for (auto &line : instructions)
	{
		Utility::updateGlobal(line);
	}
	auto _currentSelection = std::shared_ptr<Section>(nullptr);

	for (auto &line : instructions)
	{
		auto inst = 0;
		for (auto &word : line)
		{
			if (word == ".end")
			{
				return true;
			}
			if (word == ".global")
			{
				//Utility::updateGlobal(line);
				break;
			}
			if (word == ".equ")
			{
				continue;
			}
			if (word == ".align")
			{
				continue;
			}
			if (word == ".skip")
			{
				continue;
			}
			if (word == ".byte")
			{
				continue;
			}
			if (word.find(":") != std::string::npos)
			{
				continue;
			}
			if (word[0] == '.')
			{
				auto name = word;
				_currentSelection = sectionTable.find(word);
				break;
			}
			if (OperationalCodeTable::find(word))
			{
				auto bytes = OperationalCodeTable::checkInstruction(word, line);
				auto instruction = OperationalCodeTable::getInstruction(word, line);
				auto first_32 = OperationalCodeTable::constructInstruction(instruction);

				//TODO Construct word
			}
		}
	}
	return true;
}
