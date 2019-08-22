
#include <vector>
#include <fstream>

#include "../h/macros.h"
#include "../h/utility.h"
#include "../h/assembler.h"

bool Utility::readFile(std::string fileName, ArrayOfStrings &instructions)
{

    std::ifstream infile;
    infile.open(fileName);
    std::string delimetars = " , \t";
    std::string line;

    while (std::getline(infile, line))
    {
        auto tokens = Utility::tokenizeString(line, delimetars);
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
        std::string subSt = subSt.substr(lastPos, pos - lastPos);
        if (subSt.find(";") != std::string::npos)
            return tokens;
        if (subSt.find("[") != std::string::npos)
        {
            auto first = line.find_first_of("[");
            auto last = line.find_last_of("]");
            tokens.push_back(line.substr(first, last - first + 1));
            return tokens;
        }
        tokens.push_back(line.substr(lastPos, pos - lastPos));

        lastPos = line.find_first_not_of(delimetar, pos);
        pos = line.find_first_of(delimetar, lastPos);
    }
    return tokens;
}
/**
void Utility::writeFile(std::string fileName)
{
    std::ofstream outFile(fileName);

    std::string output = "#TableOfSymbols\n";
    output += Utility::secTable.to_string();

    for (auto symbol : Utility::symbTable)
    {
        if(!symbol.isSection()){
            output += symbol.to_string() +"\n";
        }
    }
    for(auto &section: Utility::secTable){
        auto table = section->getTable();
        if(table.size()){
            output+="#rel" + section->getName()+"\n";
        } 
        for(auto &entry:section->getTable())
        {
            output+= entry->to_string() +"\n";
        }
        output += section->to_string_data()+"\n";
    }
    outFile << output;
    outFile.close();
}**/