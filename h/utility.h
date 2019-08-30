#ifndef UTIL_H
#define UTIL_H

#include <regex>
#include <vector>
#include <memory>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/macros.h"
class Utility
{
public:
    static bool readFile(std::string fileName, ArrayOfStrings &instructions);
    static void writeFile(std::shared_ptr<Assembler> a,std::string fileName);
	static void updateGlobal(std::vector<std::string> & line);

public:
    //Prenosimo po referenci liniju i delimetre preko kojih hvatamo tokene
    static std::vector<std::string> tokenizeString(const std::string &str, const std::string &delimetars);
public:
    //Regex for types of adressing
    static std::regex register_regex;
    static std::regex regindpom_regex;
    static std::regex regind_regex;
    static std::regex immed_regex;
    static std::regex memdir_regex;

    //Regex for types of operands
    static std::regex instruction_regex;
};
#endif
