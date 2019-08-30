#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <vector>
#include "../h/def.h"

#define PRINT(a) std::cout<<a<<std::endl
#define READ(a) std::cin>>a

#define IMMED 0x0
#define REGDIR 0x1
#define REGIND 0x2
#define REGINDPOM8 0x3
#define REGINDPOM16 0x4
#define MEMDIR 0x5

#define ALU_INSTRUCTION 0
#define JMP_INSTRUCTION 1
#define LOAD_STORE_INSTRUCTION 2
#define STACK_INSTRUCTION 3

#define LOCAL_SYMBOL true
#define GLOBAL_SYMBOL false
#define IS_SECTION true
#define NOT_SECTION false

#define R_386_32 true
#define R_386_PC32 false

#endif