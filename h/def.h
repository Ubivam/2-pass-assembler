#ifndef DEF_H
#define DEF_H
#include "../h/table.h"
#include <vector>

class Symbol;
class Section;
typedef Table<Symbol> SymbolTable;
typedef Table<Section> SectionTable;
typedef std::vector<std::vector<std::string>> ArrayOfStrings;

#elif