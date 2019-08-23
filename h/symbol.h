#ifndef SYMBOL_H
#define SYMBOL_H
#include "../h/macros.h"
#include "../h/def.h"
#include "../h/section.h"
class Symbol{
public: 
    Symbol(std::string label, std::shared_ptr<Section> section, uint16_t offset, bool isSection = false, bool isLocal= true);

public: //SETTERS
    void setLocal(bool local);
    void setIndex(uint32_t index);
    void setOffset(uint32_t _offset);
public: //GETTERS
    std::string getName() const;
    uint32_t getIndex() const;
    uint32_t getOffset() const;
    bool isLocal() const;
    bool isSection() const;
    std::string getSectionName()const;

    std::string to_string() const;
public:
    friend std:: ostream& operator<<(std::ostream& os, const Symbol& symbol);
private:
    std::string _name;
    uint32_t _index;
    uint32_t _offset;
    bool _local;
    bool _is_section;
    std::string _sectionName;
    std::shared_ptr<Section> _section;
};
#endif