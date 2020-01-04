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
   static Code codeSegment;

private:
    static std::shared_ptr<Assembler> mInstance; 
    bool end = false;

public:
    std::shared_ptr<Section> currentSection;
public:
    Assembler();
    static std::shared_ptr<Assembler> getInstace();
   
    //Analysis Phase
    bool firstPass(ArrayOfStrings &instructions);
    void analizeUndefined();
    //Synthesis Phase
    bool secoundPass(ArrayOfStrings &instruction);

};

#endif
