#include "../h/utility.h"
#include "../h/assembler.h"
#include "../h/macros.h"
#include <fstream>

int main(int argc, char *argv[])
{
        if (argc != 3)
        {
                PRINT("You entered wrong arguments");
                return -1;
        }
        ArrayOfStrings instructions;

        if (!Utility::readFile(argv[1], instructions))
        {
                PRINT("There was an error in readingFile");
                return = -1;
        }
        //TODO: Operation Table initialize
        if (!Assembler::firstPass(instructions))
        {
                PRINT("There was an error with first Pass");
                return = -1;
        }
        if (!Assembler::secoundPass(instructions))
        {
                PRINT("There was an error with secound Pass");
                return = -1;
        }
        Utility::writeFile(argv[2]);
        PRINT("Both passes succesfully passed!");
        return 0;
}
