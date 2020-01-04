#ifndef SECTION_H
#define SECTION_H

#include "../h/macros.h"
#include <memory>
#include "../h/def.h"

class Section
{
public:
    Section(std::string name, uint32_t index, uint32_t locCounter = 0);

    void incLocationCounter(uint32_t amount);

    void resetLocationCounter();

    void appendData(uint32_t value, uint8_t size, bool firstWord = true);
	void saveAndResetLocationCounter();

    void insertRealocationEntry(std::shared_ptr<RelocationEntry>& entry);

public:
    std::string getName() const;
    uint32_t getLocationCounter() const;
    uint32_t getBeginLocationCounter() const;
    std::vector<uint8_t> getData() const;
    uint32_t getIndex() const;
    std::shared_ptr<RelocationTable>  getTable();

    std::string to_string() const;
    std::string to_string_data() const;
    std::string to_string_table() const;

private:
    std::string _stringName;
    uint32_t _beginLocCounter;
    uint32_t _locCounter;
    uint32_t _endLocCouter;
    std::vector<uint8_t> _data;
    uint32_t _index;
	std::string _flags;
	std::shared_ptr<RelocationTable> _table;
};
#endif