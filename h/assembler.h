#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <regex>

class Assembler {
    public:
    
    static bool firstPass(ArrayOfStrings &instructions);

    public:

    static bool secoundPass(ArrayOfStrings &instruction);
};

#endif
