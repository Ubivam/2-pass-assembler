#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/utility.h"
#include "../h/section.h"
#include "../h/operational_code_table.h"
#include "../h/relocation_entry.h"

SymbolTable Assembler::symbolTable;
SectionTable Assembler::sectionTable;
Code Assembler::codeSegment;
std::weak_ptr<Assembler> Assembler::mInstance;

Assembler::Assembler()
{
}

std::shared_ptr<Assembler> Assembler::getInstace()
{
	if(auto ptr = mInstance.lock()) //lock() returns a shared_ptr and increments ref count
	{
		return ptr;
	}
	auto ptr = std::shared_ptr<Assembler>(new Assembler());
	mInstance = ptr;
	return ptr;
}

//Prvi prolaz koristimo kako bismo popunili tabelu simbola
bool Assembler::firstPass(ArrayOfStrings &instructions)
{
 	currentSection = std::shared_ptr<Section>(nullptr);
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
					auto symbol = std::make_shared<Symbol>(*(it + 1), currentSection, 0, NOT_SECTION, GLOBAL_SYMBOL);
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
					auto symbol = std::make_shared<Symbol>(*(it + 1), currentSection, 0, NOT_SECTION, GLOBAL_SYMBOL);
					symbol->setIndex(uint16_t(symbolTable.size()) + 1);
					symbolTable.insert(symbol);
					it++;
				}
				continue;
			}
			if (word == ".equ")
			{
				auto symbolName = *(&word + 1);
				auto value = (uint32_t) std::stoi(*(&word + 2));
				std::shared_ptr<Symbol> symbol;
				if (currentSection == nullptr)
				{
					symbol = std::make_shared<Symbol>(symbolName, currentSection, 0 , NOT_SECTION, GLOBAL_SYMBOL);
				}
				symbol = std::make_shared<Symbol>(symbolName, currentSection, currentSection->getLocationCounter(), NOT_SECTION, LOCAL_SYMBOL);
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);
				symbol->setOffset(value);

				symbolTable.insert(symbol);
				continue;
			}
			if (word == ".byte")
			{
				if (currentSection != nullptr)
				{
					for (int i = 1; i < line.size(); i++)
					{
						currentSection->incLocationCounter(1);
						auto symbol = std::make_shared<Symbol>(line[i], currentSection, currentSection->getLocationCounter());
						symbol->setIndex(uint16_t(symbolTable.size()) + 1);
						bool exists = false;
						for (auto sym : symbolTable)
						{
							if (sym->getName() == line[i])
							{
								sym->setSection(currentSection);
								sym->setOffset(currentSection->getLocationCounter());
								exists = true;
							}
						}
						if (!exists)
						{
							symbolTable.insert(symbol);
						}
					}
				}
				continue;
			}
			if (word == ".word")
			{
				if (currentSection != nullptr)
				{
					for (int i = 1; i < line.size(); i++)
					{
						currentSection->incLocationCounter(2);
						auto symbol = std::make_shared<Symbol>(line[i], currentSection, currentSection->getLocationCounter());
						symbol->setIndex(uint16_t(symbolTable.size()) + 1);
						bool exists = false;
						for (auto sym : symbolTable)
						{
							if (sym->getName() == line[i])
							{
								sym->setSection(currentSection);
								sym->setOffset(currentSection->getLocationCounter());
								exists = true;
							}
						}
						if (!exists)
						{
							symbolTable.insert(symbol);
						}
					}
				}
				continue;
			}
			if (word == ".align")
			{
				if (!currentSection)
				{
					PRINT("Current section is nullptr");
					return false;
				}
				uint16_t val = (uint16_t)(std::stoi((*(&word + 1))));
				while (((currentSection->getLocationCounter()) % val) != 0)
				{
					currentSection->incLocationCounter(1);
				}
				continue;
			}
			if (word == ".skip")
			{
				if (currentSection != nullptr)
					currentSection->incLocationCounter(stoi(*(&word + 1)));
				continue;
			}

			/*U narednom bloku vrsimo provere da li je konkretna rec labela | instrukcija ili sekcija*/

			//Proveravamo da li je data rec labela
			if (word.find(":") != std::string::npos)
			{
				//if(inst & INSTRUCTION)
				if (!currentSection)
				{
					PRINT("Current section is nullptr");
					return false;
				}
				auto label = word.substr(0, word.length() - 1);
				auto symbol = std::make_shared<Symbol>(label, currentSection, currentSection->getLocationCounter() - currentSection->getBeginLocationCounter());
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);
				bool exists = false;
				for (auto sym : symbolTable)
				{
					if (sym->getName() == label)
					{
						sym->setSection(currentSection);
						sym->setOffset(currentSection->getLocationCounter() - currentSection->getBeginLocationCounter());
						exists = true;
					}
				}
				if (!exists)
				{
					symbolTable.insert(symbol);
				}
				continue;
			}
			// Proveravamo da li je data rec sekcija (Gore smo prosli sve slucajeve za direktive tako da izraz koji pocinje sa tackom mora da bude sekcija)
			if (word[0] == '.')
			{
				auto name = word;
				//  auto section = std::make_shared<Section>(new Section(name, _symbolTable->size() + 1, value));
				PRINT(symbolTable.size());
				auto section = std::make_shared<Section>(name, symbolTable.size() + 1, value);
				value = 0;

				sectionTable.insert(section);
				currentSection = section;

				auto symbol = std::make_shared<Symbol>(name, currentSection, currentSection->getLocationCounter(), IS_SECTION, true);
				symbol->setIndex(uint16_t(symbolTable.size()) + 1);

				symbolTable.insert(symbol);
				continue;
			}

			//Proveravamo da li je data rec instrukcija
			if (OperationalCodeTable::find(word))
			{
				auto bytes = OperationalCodeTable::checkInstruction(word, line);
				currentSection->incLocationCounter(bytes);
				continue;
			}
			//	PRINT("You have Syntax error");
			//	return false;
		}
	}
	return true;
}

bool Assembler::secoundPass(ArrayOfStrings &instructions)
{
	symbolTable.insert(std::shared_ptr<Symbol>(nullptr));
	sectionTable.insert(std::shared_ptr<Section>(nullptr));
	//Ovde smo dodali prazne elemente na kraj tabela sekcija i tabela simbola jer for each ne uzima zadnji element u obzir

	for (auto &sec : sectionTable)
	{
		sec->saveAndResetLocationCounter();
	}
	for (auto &line : instructions)
	{
		Utility::updateGlobal(line);
	}
	currentSection = std::shared_ptr<Section>(nullptr);

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

				break;
			}
			if (word == ".extern")
			{
				break;
			}
			if (word == ".equ")
			{
				continue;
			}
			if (word == ".align")
			{
				if (currentSection != nullptr)
				{
					uint16_t val = (uint16_t) (std::stoi((*(&word + 1))));
					while (((currentSection->getLocationCounter()) % val) != 0)
					{
						currentSection->incLocationCounter(1);
						Instruction ins;
						ins.push_back(0x00);
						codeSegment.push_back(ins);
						currentSection->inserInstruction(ins);
					}
					continue;
				}
				continue;
			}
			if (word == ".skip")
			{
				if (currentSection != nullptr)
				{
					auto num_of_bytes = stoi(*(&word + 1));
					currentSection->incLocationCounter(num_of_bytes);
					Instruction ins;
					for (int i = 0; i < num_of_bytes; i++)
					{
						ins.push_back(0x00);
					}
					codeSegment.push_back(ins);
					currentSection->inserInstruction(ins);
				}
				continue;
			}
			if (word == ".byte")
			{
				if (currentSection != nullptr)
				{
					for (int i = 1; i < line.size(); i++)
					{
						currentSection->incLocationCounter(1);
						Instruction ins;
						ins.push_back(0x00);
						codeSegment.push_back(ins);
						currentSection->inserInstruction(ins);
					}
				}
				continue;
			}
			if (word == ".word")
			{
				if (currentSection != nullptr)
				{
					for (int i = 1; i < line.size(); i++)
					{
						currentSection->incLocationCounter(2);
						Instruction ins;
						ins.push_back(0x00);
						ins.push_back(0x00);
						codeSegment.push_back(ins);
						currentSection->inserInstruction(ins);
					}
				}
				continue;
			}
			if (word.find(":") != std::string::npos)
			{
				continue;
			}
			if (word[0] == '.')
			{
				auto name = word;
				currentSection = sectionTable.find(name);
				break;
			}
			if (OperationalCodeTable::find(word))
			{
				auto bytes = OperationalCodeTable::checkInstruction(word, line);
				currentSection->incLocationCounter(bytes);
				auto instruction = OperationalCodeTable::getInstruction(word, line);
				auto binary_instruction = OperationalCodeTable::constructInstruction(instruction);

				codeSegment.push_back(binary_instruction);
				currentSection->inserInstruction(binary_instruction);
			}
		}
	}
	return true;
}
