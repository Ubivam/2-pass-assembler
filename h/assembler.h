#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <regex>
#include "../h/section.h"
#include "../h/symbol.h"

class Assembler
{
public:
    SymbolTable _symbolTable;
    SectionTable _sectionTable;
private:
    bool _globalFlag;
    bool _externFlag;
    bool end = false;

public:
    Assembler();
    //Analysis Phase
    bool firstPass(ArrayOfStrings &instructions);
    void analizeUndefined();

public:
    //Synthesis Phase
    bool secoundPass(ArrayOfStrings &instruction);
};

#endif
