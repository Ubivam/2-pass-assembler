#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <regex>

class Assembler {
    public:
    
    static bool firstPass(std::vector<std::vector<std::string>> &instructions);

    public:

    static bool secoundPass(std::vector<std::vector<std::string>> &instruction);
};

#endif
