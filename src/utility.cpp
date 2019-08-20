#include <vector>
#include "../h/macros.h"
#include "../h/utility.h"
#include "../h/assembler.h"

bool Utility::readFile(std::string fileName, ArrayOfStrings &instructions)
{

    std::ifstream input;
    input.open(fileName);
    std::string delimetars = " , \t";
    std::string line;

    while (std::getline(input, line))
    {
        auto tokens = Utility.tokenizeString(line, delimetars);
        if (tokens.empty())
            continue;
        instructions.push_back(tokens);
    }
    infile.close();
    if (instructions.size() == 0)
    {
        PRINT("The file you provided is empty!");
        return false;
    }
    return true;
}

std::vector<std::string> Utility::tokenizeString(const std::string &line, const std::string &delimetar)
{
    std::vector<std::string> tokens;
    if (line.empty())
        return tokens;
    //Preskoci prvi delimetar na pocetku
    std::string::size_type lastPos = line.find_first_not_of(delimetar, 0);
    std::string::size_type pos = line.find_first_of(delimetar, lastPos);

    // std::string::npos means unitl the end of the string
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        std::string subSt= subSt.substr(latest, pos - lastPos);
        if(subSt.find())
    }
}