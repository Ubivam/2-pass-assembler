#include "../h/utility.h"
#include "../h/assembler.h"
#include "../h/macros.h"
#include "../h/table.h"
#include "../h/operational_code_table.h"
#include <fstream>

int main(int argc, char *argv[])
{
        if (argc != 3)
        {
                PRINT("You entered wrong arguments");
                return -1;
        }
        //Initialization Block

        ArrayOfStrings instructions;
        auto assembler = std::shared_ptr<Assembler>();
        OperationalCodeTable::init();
        if (!Utility::readFile(argv[1], instructions))
        {
                PRINT("There was an error in readingFile");
                return -1;
        }
        if (assembler->firstPass(instructions))
        {
                PRINT("There was an error with first Pass");
                return -1;
        }
        if (assembler->secoundPass(instructions))
        {
                PRINT("There was an error with secound Pass");
                return -1;
        }
        Utility::writeFile(argv[2]);
        PRINT("Both passes succesfully passed!");
        return 0;
}
