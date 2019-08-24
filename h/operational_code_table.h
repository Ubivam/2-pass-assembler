#ifndef OPERATIONAL_CODE_TABELE_H
#define OPERATIONAL_CODE_TABELE_H
#include "../h/mnemonic.h"
#include "../h/section.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>


class OperationalCodeTable
{
private:
    static std::unordered_map<std::string, std::shared_ptr<Mnemonic>> _map;

public:
    static void init();

    static bool find(std::string op);
    static std::shared_ptr<Mnemonic> get(std::string op);

    static uint8_t getAddressMode(std::vector<std::string>& instruction);
    static uint8_t getAddressModes(std::string operation);
    
    static std::vector<std::string> getInstruction(std::string operation,std::vector<std::string> line);

    static uint8_t checkInstruction(std::string op, std::vector<std::string> line);
    static uint32_t constructInstruction(std::vector<std::string> inst);
    static uint32_t constructSecountWord(std::vector<std::string> inst, std::shared_ptr<Section> currentSection);
};
#endif