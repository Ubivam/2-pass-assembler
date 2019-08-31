
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

std::vector<std::string> Utility::tokenizeString(const std::string& str, const std::string& delimiters)
{
	std::vector<std::string> tokens;
	if (str.empty())return tokens;
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{  // Found a token, add it to the vector.
		std::string sub = str.substr(lastPos, pos - lastPos);
		if (sub.find(";") != std::string::npos) return tokens;
		if (sub.find("[") != std::string::npos)
		{
			auto first = str.find_first_of("[");
			auto last = str.find_last_of("]");
			tokens.push_back(str.substr(first, last - first + 1));
			return tokens;
		}
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}
void Utility::writeFile(std::shared_ptr<Assembler> a, std::string fileName)
{
    std::ofstream outFile(fileName);

    std::string output = "#TableOfSections\n";
    output += a->sectionTable.to_string();
    
	output += "#TableOfSymbols\n";
	a->symbolTable.insert(std::shared_ptr<Symbol>(nullptr));
    for (auto symbol : a->symbolTable)
    {
        if(!symbol->isSection()){
            output += symbol->to_string() +"\n";
        }
    }
    /**for(auto &section: Utility::secTable){
        auto table = section->getTable();
        if(table.size()){
            output+="#rel" + section->getName()+"\n";
        } 
        for(auto &entry:section->getTable())
        {
            output+= entry.to_string() +"\n";
        }
        output += section->to_string_data()+"\n";
    }**/
    outFile << output;
    outFile.close();
}
void Utility::updateGlobal(std::vector<std::string>& line) 
{
	auto global = false;
	for (auto& word2 : line)
	{
		if (global)
		{
			auto symbol = Assembler::symbolTable.find(word2);
			symbol->setLocal(GLOBAL_SYMBOL);
		}
		if (word2 == ".global") global = true;
	}
}

//Register Regex
std::regex Utility::register_regex("^(R([0-9]||1[0-5])||SP||PC||PSW)$");
std::regex Utility::regindpom_regex("^\\[(R([0-9]||1[0-5])||PC||SP) *(\\+||\\-) *(0x||0b)?[a-zA-z0-9]+\\]$");
std::regex Utility::regind_regex("^\\[(R([0-9]||1[0-5])||PC||SP)\\]$");
std::regex Utility::immed_regex("^#([0-9A-F]+||[a-zA-z][a-zA-Z0-9]*)");
std::regex Utility::memdir_regex("^\\$?[a-zA-Z][a-zA-Z0-9]*$");

//Instruction Regex
std::regex Utility::instruction_regex("(INT|ADD|SUB|MUL|MOV|DIV|CMP|AND|OR|NOT|TEST|XOR|XCHG|CALL|SHR|SHL|HALT|RET|IRET|POP|PUSH|JMP|JEQ|JNE|JGT)(B|W)?");