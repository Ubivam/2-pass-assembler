#include "../h/section.h"
#include "../h/symbol.h"
#include "../h/def.h"
#include "../h/macros.h"
#include <sstream>

Symbol ::Symbol(std::string label, std::shared_ptr<Section> section, uint16_t offset, bool isSection, bool isLocal)
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
bool Symbol::isSection() const
{
    return _isSection;
}
uint32_t Symbol::getIndex() const
{
    return _index;
}
std::string Symbol::to_string() const
{
    std::stringstream stream;
    stream << "0x" << std::uppercase << std::hex << _offset;
    std::string value(stream.str());
    std::string index = std::to_string(_section->getIndex());
    return "SYM" + std::to_string(_index) + " " + _name + " " + index + " " + value + " " + (_local ? "G" : "L");
}
std::ostream &operator<<(std::ostream &os, const Symbol &symbol)
{
    if (!symbol.isSection())
        os << symbol.to_string() << std::endl;
    return os;
}
