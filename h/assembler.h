#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <regex>
#include "../h/selection.h"

class Assembler
{
private:
    std::shared_ptr<SymbolTable> symbolTable;
    std::shared_ptr<Selection> selection;
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
