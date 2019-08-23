#include <vector>
#include <fstream>
#include <memory>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/utility.h"
#include "../h/section.h"
#include "../h/operational_code_tabele.h"
bool Assembler::firstPass(ArrayOfStrings &instructions)
{
    currentSection = std::shared_ptr<Section>(nullptr);

    uint16_t value = 0;
    bool def_flag = false;
    bool dup_flag = false;
    bool org = false;

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
                break;

            if (word == ".equ")
            {
            }
            if (word == ".byte")
            {
            }
            if (word == ".align")
            {
            }
            if (word == ".skip")
            {
            }
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
                symbol->setIndex(uint16_t(this->symbolTable->size()) + 1);
                symbolTable->insert(symbol);
                continue;
            }
            // if it is a section
            if (word[0] == '.')
            {
                auto name = word;
                auto section = std::make_shared<Section>(name, symbolTable->size() + 1, value);
                value = 0;

                sectionTable->insert(section);
                currentSection = section;

                auto symbol = std::make_shared<Symbol>(name,currentSection,currentSection->getLocationCounter());
                symbol->setIndex(uint16_t(symbolTable->size()) + 1);
                symbolTable->insert(symbol);
                break;
            }
            //Is it instruction
            if (OperationalCodeTabele::find(word))
            {

            }
        }
    }
    return true;
}

Assembler::Assembler()
{
    symbolTable = std::shared_ptr<SymbolTable>();
}