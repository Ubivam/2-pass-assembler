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

#endif