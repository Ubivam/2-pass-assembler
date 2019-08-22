#include <vector>
#include <fstream>
#include <memory>
#include "../h/macros.h"
#include "../h/assembler.h"
#include "../h/utility.h"
#include "../h/selection.h"
#include "../h/symbolTable.h"

bool Assembler::firstPass(ArrayOfStrings &instructions)
{
    
}

Assembler::Assembler()
{
    selection = std::shared_ptr<Selection>();
    symbolTable = std::shared_ptr<SymbolTable>();
}