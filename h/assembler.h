#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <regex>
#include "../h/section.h"
#include "../h/symbol.h"

class Assembler
{
public:
   static SymbolTable symbolTable;
   static SectionTable sectionTable;
private:
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
