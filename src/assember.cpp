#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/utility.h"
#include "../h/section.h"
#include "../h/operational_code_table.h"


SymbolTable Assembler::testSymbol; 

Assembler::Assembler()
{
   _symbolTable = std::make_shared<SymbolTable>();
   _sectionTable= std::make_shared<SectionTable>();
}

bool Assembler::firstPass(ArrayOfStrings &instructions)
{
    PRINT(Assembler::testSymbol.size());
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
                // _symbolTable->insert(std::make_shared<Section>(word, _symbolTable->size() + 1, _currentSection, _currentSection->getLocationCounter(), false, false));
                break;
            if (word == ".extern")
            {
                // _symbolTable->insert(std::make_shared<Section>(word, _symbolTable->size() + 1, _currentSection, _currentSection->getLocationCounter(), false, true));
                break;
            }
            if (word == ".equ")
            {
            }
            if (word == ".byte")
            {
                while (&word != &line.back())
                {
                    _symbolTable->insert(std::make_shared<Symbol>(word, _currentSection, _currentSection->getLocationCounter()));
                    _currentSection->incLocationCounter(1);
                }
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
            }
            if (word == ".skip")
            {
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
                symbol->setIndex(uint16_t(_symbolTable->size()) + 1);
                _symbolTable->insert(symbol);
                continue;
            }
            // if it is a section
            if (word[0] == '.')
            {
                auto name = word;
              //  auto section = std::make_shared<Section>(new Section(name, _symbolTable->size() + 1, value));
                auto section = std::make_shared<Section>(name, _symbolTable->size() + 1, value);              
                value = 0;

                _sectionTable->insert(section);
                _currentSection = section;

                auto symbol = std::make_shared<Symbol>(name, _currentSection, _currentSection->getLocationCounter());
                symbol->setIndex(uint16_t(_symbolTable->size()) + 1);
                _symbolTable->insert(symbol);
                break;
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
                break;
            }
        }
    }
    return true;
}

bool Assembler::secoundPass(ArrayOfStrings &instructions)
{
    return true;
}
