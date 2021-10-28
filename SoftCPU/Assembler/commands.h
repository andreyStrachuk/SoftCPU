#ifndef COMMANDS_H
#define COMMANDS_H

static char N_HLT [] = "hlt";
static char N_PUSH [] = "push";
static char N_POP [] = "pop";
static char N_ADD [] = "add";
static char N_MUL [] = "mul";
static char N_SUB [] = "sub";
static char N_OUT [] = "out";
static char N_IN [] = "in";
static char N_SQRT [] = "sqrt";
static char N_DIV [] = "div";

static char N_AX [] = "ax";
static char N_BX [] = "bx";
static char N_CX [] = "cx";
static char N_DX [] = "dx";

enum TYPEOFREGISTER {
    AX = 0x01,
    BX = 0x02,
    CX = 0x03,
    DX = 0x04
};

struct COMMAND {
    unsigned char mem : 1;
    unsigned char reg : 1;
    unsigned char imm : 1;
    unsigned char cmd : 5;
};

enum COMMANDS_NUMBERS {
    HLT,
    PUSH,
    POP,
    ADD,
    MUL,
    SUB,
    OUT,
    IN,
    SQRT,
    DIV
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
    ZERO_DIV = -6
};

#endif