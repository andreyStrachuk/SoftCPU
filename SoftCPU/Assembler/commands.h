#ifndef COMMANDS_H
#define COMMANDS_H

extern char N_PUSH [];
extern char N_POP []; 

enum TYPEOFREGISTER {
    #define DEF_REG_(name, number) R_##name = number,

    #include "regs_def.h"

    #undef DEF_REG_
};

struct COMMAND {
    unsigned char mem : 1;
    unsigned char reg : 1;
    unsigned char imm : 1;
    unsigned char cmd : 5;
};

struct Label {
    int ip;
    char *txt;
};

enum COMMANDS_NUMBERS {
    
    #define DEF_CMD_(name, numbOfCmd, argNumber, code)  CMD_##name = numbOfCmd,                                           \

    #include "commands_def.h"

    #undef DEF_CMD_
};

enum TYPEOFWRITE {
    NTHG,
    IMM,
    REG,
    MEM,
    MEMREG,
    MEMREGIMM
};

enum ERRORS {
    UNKNOWN_COMMAND = -10,
    UNKNOWN_REGISTER = -9,
    INCORRECT_INPUT = -8,
    OK = -7,
    ZERO_DIV = -6,
    WRONG_ADDRESS = -5,
    FIRST,
    SECOND,
    NULLPTR,
    UNABLETOREADFROMFILE
};

#endif