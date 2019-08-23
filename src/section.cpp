#include "../h/section.h"
#include <vector>
#include <sstream>
#include <iomanip>

Section::Section(std::string name, uint32_t index, uint32_t locCounter)
    : _stringName(name),
      _index(index),
      _beginLocCounter(locCounter),
      _endLocCouter(locCounter),
      _locCounter(locCounter)
{
    if (name.find("text") != std::string::npos)
    {
        _flags = "XPA";
    }
    if (name.find("data") != std::string::npos)
    {
        _flags = "WPA";
    }
    if (name.find("bss") != std::string::npos)
    {
        _flags = "WA";
    }
}
void Section::incLocationCounter(uint32_t amount)
{
    _locCounter += amount;
}
void Section::resetLocationCounter()
{
    _locCounter = 0;
}
void Section::appendData(uint32_t value, uint8_t size, bool firstWord)
{
    if (firstWord)
    {
        auto shiftBy = 24;
        while (shiftBy >= 0)
        {
            auto byte = value >> shiftBy;
            _data.push_back(uint8_t(byte));
            shiftBy -= 8;
        }
    }
    else
    {
        for (auto i = 0; i < size; i++)
        {
            _data.push_back(u_int8_t(value));
            value >>= 8;
        }
    }
    _locCounter += size;
}
void Section::saveAndResetLocCounter(std::shared_ptr<RelocationEntry> &entry)
{
    //TODO: Vidi ovo sta kako
    _endLocCouter = _locCounter;
    _locCounter = _beginLocCounter;
}

std::string Section::getName() const
{
    return _stringName;
}
uint32_t Section::getLocationCounter() const
{
    return _locCounter;
}
uint32_t Section::getBeginLocationCounter() const
{
    return _beginLocCounter;
}
std::vector<uint8_t> Section::getData() const
{
    return _data;
}
uint32_t Section::getIndex() const
{
    return _index;
}

std::string Section::to_string() const
{
    std::stringstream stream;
    stream << "0x" << std::uppercase << std::hex << _beginLocCounter;
    auto begin = stream.str();

    stream.str(std::string());

    stream << "0x" << std::uppercase << std::hex << _endLocCouter - _beginLocCounter;
    auto end = stream.str();

    return "SEG" + std::to_string(_index) + " " + _stringName + " " + std::to_string(_index) + " " + begin + " " + end + " " + _flags;
}