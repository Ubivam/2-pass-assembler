#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <vector>
#include "../h/def.h"

#define PRINT(a) std::cout<<a<<std::endl
#define READ(a) std::cin>>a

//Sirina podatka
#define LOW 0x0
#define HIGH 0x1

//Tipovi adresiranja
#define IMMED 0x0
#define REGDIR 0x1
#define REGIND 0x2
#define REGINDPOM 0x3
#define REGINDPOM16 0x4
#define MEMDIR 0x5

//Pomocni tipovi adresiranja
#define IMMED_SYM 0x6
#define REGINDPOM_SYM 0x7
#define PC_RELATIVE_SYM 0x8
#define ABS_SYM 0x9

#define ALU_INSTRUCTION 0
#define JMP_INSTRUCTION 1
#define LOAD_STORE_INSTRUCTION 2
#define STACK_INSTRUCTION 3
#define CONTROL_INSTRUCTION 4

#define LOCAL_SYMBOL true
#define GLOBAL_SYMBOL false
#define IS_SECTION true
#define NOT_SECTION false

//Apsolutna adresa za realokaciju simbola stavlja 32 bitnu adresu simbola u memorijsku lokaciju
#define R_386_32 true
//PC Relativna adresa za realokaciju simbola stavlja 32 bitnu realtivnu adresu simbola u memorijsku lokaciju
#define R_386_PC32 false

#endif