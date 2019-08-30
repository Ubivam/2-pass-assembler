#ifndef RELOCATION_ENTRY
#define RELOCATION_ENTRY

#include "../h/relocation_entry.h"
#include "../h/def.h"
#include "../h/macros.h"

RelocationEntry::RelocationEntry(uint32_t offset, RelocationType type, uint32_t index):
_offset(offset),
_type (type),
_index (index)
{
}

std::string RelocationEntry::to_string() const
{
	std::stringstream stream;
	std::string type = (_type == R_386_32) ? "ABS" : "REL";
	stream << "0x" << std::uppercase << std::hex << _offset << " " << type << " " << std::dec << _index;
	auto s = stream.str();
	return s;
}

#endif

std::ostream& operator<<(std::ostream& os, RelocationEntry& entry)
{
	os << entry.to_string();
	return os;
}
