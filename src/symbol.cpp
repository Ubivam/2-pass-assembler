#include "../h/section.h"
#include "../h/symbol.h"
#include "../h/def.h"
#include "../h/macros.h"
#include <sstream>

Symbol::Symbol()
{

}
Symbol::Symbol(std::string label, std::shared_ptr<Section> section, uint16_t offset, bool isSection, bool isLocal)
	: _name(label),
	_section(section),
	_offset(offset),
	_isSection(isSection),
	_local(isLocal),
	_index(0)
{
}
void Symbol::setIndex(uint32_t index)
{
	_index = index;
}
void Symbol::setLocal(bool local)
{
	_local = local;
}
void Symbol::setOffset(uint32_t offset)
{
	_offset = offset;
}
std::string Symbol::getName() const
{
	return _name;
}
std::shared_ptr<Section> Symbol::getSection() const
{
	return _section;
}
bool Symbol::isLocal() const
{
	return _local;
}
bool Symbol::isSection() const
{
	return _isSection;
}
uint32_t Symbol::getIndex() const
{
	return _index;
}
uint32_t Symbol::getOffset() const
{
	return _offset;
}
std::string Symbol::to_string() const
{
	std::stringstream stream;
	stream << "0x0" << std::uppercase << std::hex << _offset;
	std::string value(stream.str());
	if (_section != nullptr)
	{
		std::string section = _section->getName();
		return "SYM:\t" + std::to_string(_index) + "\t\t" + _name + "\t\t" + section + "\t" + value + "\t" + (_local ? "L" : "G");
	}
	else
	{
		return "SYM:\t" + std::to_string(_index) + "\t\t" + _name + "\t\t" + "NULL" + "\t\t" + value + "\t" + (_local ? "L" : "G");
	}
}
std::ostream& operator<<(std::ostream & os, const Symbol & symbol)
{
	if (!symbol.isSection())
		os << symbol.to_string() << std::endl;
	return os;
}
