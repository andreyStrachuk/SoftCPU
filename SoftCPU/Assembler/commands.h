#ifndef COMMANDS_H
#define COMMANDS_H

static const char N_HLT [] = "hlt"; // const
static char N_PUSH [] = "push"; // N_PUSH new naming
static char N_POP [] = "pop";
static char N_ADD [] = "add";
static char N_MUL [] = "mul";
static char N_SUB [] = "sub";
static char N_OUT [] = "out";
static char N_IN [] = "in";
static char N_SQRT [] = "sqrt";
static char N_DIV [] = "div";
static char N_COS [] = "cos";
static char N_SIN [] = "sin";
static char N_JMP [] = "jmp";
static char N_DEC [] = "dec";

static char N_AX [] = "ax";
static char N_BX [] = "bx";
static char N_CX [] = "cx";
static char N_DX [] = "dx";

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
    MEM
};

enum ERRORS {
    UNKNOWN_COMMAND = -10,
    UNKNOWN_REGISTER = -9,
    INCORRECT_INPUT = -8,
    OK = -7,
    ZERO_DIV = -6,
    WRONG_ADDRESS = -5,
    FIRST,
    SECOND
};

#endif