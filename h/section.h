#ifndef SECTION_H
#define SECTION_H

#include "../h/macros.h"
#include "../h/def.h"

class Section
{
public:
    Selection(std::string name, uint32_t index,uint32_t locctr = 0, bool org = false);

    void incLocationCounter(uint32_t amount);

    void resetLocationCounter();

    void appendData(uint32_t value, uint8_t size, bool firstWord=true);

public:

private:
    std::_stringName;
    uint32_t _beginLocCounter;
    uint32_t _locCounter;
    uint32_t _endLocCouter;
    std::vector<uint8_t> _data;
    uint32_t _index;
    std::string _flags;
    RelocationTable _table;
};
#endif