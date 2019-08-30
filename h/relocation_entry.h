#ifndef RELOCATION_ENTRY_H
#define RELOCATION_ENTRY_H

#include "../h/macros.h"
#include "../h/def.h"
#include <sstream>

class RelocationEntry
{
private:
	uint32_t _offset;
	RelocationType _type;
	uint32_t _index;
public:
	RelocationEntry(uint32_t offset, RelocationType type, uint32_t index);

	friend std::ostream& operator <<(std::ostream& os, RelocationEntry& entry);
	std::string to_string() const;
};


#endif