
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
    for (auto& section : a->sectionTable)
    {
        for (auto& instruction : section->getData())
        {
            std::stringstream ss;
            for (auto& word : instruction)
            {
                if (word <= 0xf) ss << std::hex << 0;
                ss << std::hex << static_cast<int>(word) << " ";
            }
            ss << "|";
            i++;
            if ((i % 3) == 0)
            {
                ss << "\n";
            }
            output += ss.str();
        }
    }
    outFile << output;
    outFile.close();
}

void Utility::writeBinFile(std::string fileName)
{
    std::ofstream outFile(fileName, std::ofstream::binary);
    auto a = Assembler::getInstace();
    uint32_t size = 0;
    auto & sec_table = a->sectionTable;
    size = sec_table.size();
    outFile.write(reinterpret_cast<char *>(&size), sizeof(size));
    uint32_t str_size;
    std::string name;
    for(auto &sec : sec_table)
    {    
        std::string  name = sec->getName();
        str_size = name.size();
        outFile.write(reinterpret_cast<char *>(&str_size), sizeof(uint32_t));
        outFile.write(&name[0], str_size);
        uint32_t index = sec->getIndex();
        outFile.write(reinterpret_cast<char*>(&index),sizeof(uint32_t));
        uint32_t begin_loc = sec->getBeginLocationCounter();
        outFile.write(reinterpret_cast<char*>(&begin_loc), sizeof(uint32_t));
        uint32_t end_loc = sec->getEndLocationCounter();
        outFile.write(reinterpret_cast<char*>(&end_loc), sizeof(uint32_t));
        
        std::shared_ptr<RelocationTable> real_tab =  sec->getTable();
        uint32_t realoc_size = real_tab->size();
        outFile.write(reinterpret_cast<char*>(&realoc_size), sizeof(uint32_t));
        for(auto reloc : real_tab->getTable())
        {
            uint32_t rel_ind = reloc->getIndex();
            outFile.write(reinterpret_cast<char*>(&rel_ind), sizeof(uint32_t));
            uint32_t rel_off = reloc->getOffset();
            outFile.write(reinterpret_cast<char*>(&rel_off), sizeof(uint32_t));
            RelocationType rel_type = reloc->getType();
            outFile.write(reinterpret_cast<char*>(&rel_type), sizeof(RelocationType));
        } 
        Code section_code = sec->getData();
        size = section_code.size();
        outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
        for (auto inst : section_code)
        {
            uint32_t size_of_instruction = inst.size();
            outFile.write(reinterpret_cast<char*>(&size_of_instruction), sizeof(size_of_instruction));
            for (auto word : inst)
            {
                outFile.write(reinterpret_cast<char*>(&word), sizeof(word));
            }

        }
    }
    auto & symb_table = a->symbolTable;
    size = symb_table.size();
    outFile.write(reinterpret_cast<char *>(&size), sizeof(size));
    for(auto &symb : symb_table)
    {
        name = symb->getName();
        str_size = name.size();
        outFile.write(reinterpret_cast<char *>(&str_size), sizeof(uint32_t));
        outFile.write(&name[0], str_size);
        uint32_t index = symb->getIndex();
        outFile.write(reinterpret_cast<char*>(&index),sizeof(uint32_t));
        uint32_t offset = symb->getOffset();
        outFile.write(reinterpret_cast<char*>(&offset),sizeof(uint32_t));
        bool local = symb->isLocal();
        outFile.write(reinterpret_cast<char*>(&local),sizeof(bool));
        bool isSection = symb->isSection();
        outFile.write(reinterpret_cast<char*>(&isSection),sizeof(bool));
        std::string section_name; 
        if(symb->getSection() != nullptr)
        {
            section_name = symb->getSection()->getName();
        }
        else
        {
            section_name = "NULL";
        }
        str_size = section_name.size();
        outFile.write(reinterpret_cast<char *>(&str_size), sizeof(uint32_t));
        outFile.write(&section_name[0], str_size);
    }
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