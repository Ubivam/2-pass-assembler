
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

		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}
void Utility::writeFile(std::string fileName)
{
    std::ofstream outFile(fileName);
    auto a = Assembler::getInstace();
    std::string output = "###### SECTION TABLE ######\n";
    output += "\t\tINDEX\tNAME\tLOC\t\tBEGIN\tEND\n";

    for (auto section : a->sectionTable)
    {
        output += section->to_string() + '\n';
    }
	output += "###### SYMBOL TABLE ######\n";
    output += "\t\tINDEX\tNAME\tSECTION\t\tOFFSET\tSCOPE\n";
    for (auto symbol : a->symbolTable)
    {
        if(!symbol->isSection()){
            output += symbol->to_string() +"\n";
        }
    }
    output += "###### RELOCATION TABLE ######\n";
    output += "\t\tSECTION\t\tINDEX\tTYPE\tOFFSET\n";
    for(auto &section : a->sectionTable){
        auto relTable = section->getTable();
        for(auto entry : relTable->getTable())
        {
            output += "REL:\t" + section->getName() + "\t" + entry->to_string() + "\n";
        }
    }
    output += "###### CODE ######\n";
    int i = 0;
    for(auto &instruction : a->codeSegment)
    {
        std::stringstream ss;
        for(auto &word : instruction)
        {
            if(word <= 0xf) ss << std::hex << 0;
            ss << std::hex << static_cast<int>(word) << " ";
        }
        ss << "|";
        i++;
        if((i % 3)== 0)
        {
            ss << "\n";
        }
        output += ss.str();
    }
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
bool Utility::isNumber(std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//Register Regex
std::regex Utility::immed_regex_value("([0-9A-F]*)");

std::regex Utility::immed_regex_simbol("\\&(\\w*)");

std::regex Utility::register_regex("^(r([0-9]||1[0-5])||sp||pc||psw)$");
std::regex Utility::regind_regex("^\\[(r([0-9]||1[0-5])||pc||sp)\\]$");
std::regex Utility::regindpom_regex("^r(([0-9]||1[0-5])||pc||sp)\\[[0-9A-F]*\\]$");

std::regex Utility::regindpom_regex_simbol("^(r([0-9]||1[0-5])||pc||sp)\\[([a-zA-Z]*)\\]$");
std::regex Utility::pc_relative_regex_simbol("\\$(\\w*)");
std::regex Utility::abs_regex_simbol("([a-zA-Z]*)");

std::regex Utility::memdir_regex("\\*([0-9A-F]+||[a-zA-z][a-zA-Z0-9]*)");

//Instruction Regex
std::regex Utility::instruction_regex("(int|add|sub|mul|mov|div|cmp|and|or|not|test|xor|xcfg|call|shr|shl|halt|ret|iret|pop|push|jmp|jeq|jne|jgt)(b|w)?");