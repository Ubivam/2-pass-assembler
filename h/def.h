#ifndef DEF_H
#define DEF_H
#include "../h/table.h"
#include <vector>

class Symbol;
class Section;
class Literal;
class RelocationEntry;
class Mnemonic;

typedef Table<Symbol> SymbolTable;
typedef Table<Section> SectionTable;
typedef Table<Literal> LiteralTable;
typedef Table<RelocationEntry> RelocationTable;
typedef std::vector<uint8_t>  Instruction;
typedef std::vector<std::vector<std::string>> ArrayOfStrings;
typedef bool RelocationType;

#endif