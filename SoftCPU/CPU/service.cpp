#include "service_functions.h"
#include "../Assembler/commands.h"
#include "../Assembler/general_functions.h"
#include "processor.h"

char N_PUSH [] = "push";
char N_POP [] = "pop";

#define DEF_REG_(reg, code) \
            case R_##reg: { \
                return softCPU->reg; \
            } \


double GetRegValue (int reg, CPU *softCPU) {
    switch (reg)
    {
        #include "../Assembler/regs_def.h"
        
        default:
            return UNKNOWN_REGISTER;
    }
}

#undef DEF_REG_

#define DEF_REG_(reg, code) \
        case R_##reg: { \
            softCPU->reg = val; \
            break; \
        } \


int WriteToRegister (int regType, CPU *softCPU, double val) {
    assert (softCPU);

    switch (regType) {
        #include "../Assembler/regs_def.h"

        #undef DEF_REG_

        default : 
            return UNKNOWN_REGISTER;
    }

    return OK;

}

int CheckIfImm (const int type) {
    if (type & 0x20 && !(type & 0x40) && !(type & 0x80)) {
        return IMM;
    }
    
    return 0;
}

int CheckIfReg (const int type) {
    if (type & 0x40 && !(type & 0x80) && !(type & 0x20)) {
        return REG;
    }

    return 0;
}

int CheckIfMem (const int type) {
    if (type & 0x80 && !(type & 0x40) && !(type & 0x20)) {
        return MEM;
    }
    else if (type & 0x80 && type & 0x40 && !(type & 0x20)) {
        return MEMREG;
    }
    else if (type & 0x80 && type & 0x40 && type & 0x20) {
        return MEMREGIMM;
    }

    return 0;
}

#define DEF_CMD_(name, cmdNumb, argNumber, code, function)                                                        \
                    case CMD_##name: {                                                                  \
                        switch (argNumber) {                                                            \
                            case 0:                                                                     \
                                code                                                                    \
                                continue;                                                               \
                            case 1: {                                                                   \
                                res = CheckIfImm (type);                                                \
                                if (res == IMM) {                                                       \
                                    if (STR_EQ (#name, N_PUSH)) {                                       \
                                        val = *(double *)(softCPU->machineCode + softCPU->ip);          \
                                                                                                        \
                                        code                                                            \
                                                                                                        \
                                        softCPU->ip += sizeof (double);                                 \
                                        continue;                                                       \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_POP)) {                                        \
                                        PopStack (softCPU->st);                                         \
                                    }                                                                   \
                                    continue;                                                           \
                                }                                                                       \
                                                                                                        \
                                res = CheckIfReg (type);                                                \
                                if (res == REG) {                                                       \
                                    int reg = *(char *)(softCPU->machineCode + softCPU->ip);            \
                                    softCPU->ip++;                                                      \
                                    if (STR_EQ (#name, N_PUSH)) {                                       \
                                                                                                        \
                                        val = GetRegValue (reg, softCPU);                               \
                                                                                                        \
                                        PushStack (softCPU->st, val);                                   \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_POP)) {                                        \
                                                                                                        \
                                        val = PopStack (softCPU->st);                                   \
                                                                                                        \
                                        res = WriteToRegister (reg, softCPU, val);                      \
                                                                                                        \
                                        if (res == UNKNOWN_REGISTER) {                                  \
                                            return UNKNOWN_REGISTER;                                    \
                                        }                                                               \
                                    }                                                                   \
                                    continue;                                                           \
                                }                                                                       \
                                                                                                        \
                                res = CheckIfMem (type);                                                \
                                if (res == MEM) {                                                       \
                                    shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);         \
                                                                                                        \
                                    softCPU->ip += 2;                                                   \
                                                                                                        \
                                    if (shift > RAMVOLUME) {                                            \
                                        return WRONG_ADDRESS;                                           \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_PUSH)) {                                       \
                                                                                                        \
                                        val = softCPU->RAM [shift];                                     \
                                                                                                        \
                                        PushStack (softCPU->st, val);                                   \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_POP)) {                                        \
                                                                                                        \
                                        val = PopStack (softCPU->st);                                   \
                                                                                                        \
                                        softCPU->RAM [shift] = val;                                     \
                                    }                                                                   \
                                    continue;                                                           \
                                }                                                                       \
                                if (res == MEMREG) {                                                    \
                                    int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);      \
                                                                                                        \
                                    softCPU->ip++;                                                      \
                                                                                                        \
                                    index = GetRegValue (typeOfReg, softCPU);                           \
                                                                                                        \
                                    if (index > RAMVOLUME) {                                            \
                                        return WRONG_ADDRESS;                                           \
                                    }                                                                   \
                                                                                                        \
                                    if (STR_EQ (#name, N_PUSH)) {                                       \
                                        val = softCPU->RAM [index];                                     \
                                                                                                        \
                                        PushStack (softCPU->st, val);                                   \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_POP)) {                                        \
                                        val = PopStack (softCPU->st);                                   \
                                                                                                        \
                                        softCPU->RAM [index] = val;                                     \
                                    }                                                                   \
                                    continue;                                                           \
                                }                                                                       \
                                if (res == MEMREGIMM) {                                                 \
                                    int typeOfReg = *(softCPU->machineCode + softCPU->ip);              \
                                                                                                        \
                                    softCPU->ip++;                                                      \
                                                                                                        \
                                    double regValue = GetRegValue (typeOfReg, softCPU);                 \
                                                                                                        \
                                    shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);         \
                                    softCPU->ip += 2;                                                   \
                                                                                                        \
                                    index = (int)regValue + shift;                                      \
                                                                                                        \
                                    if (index > RAMVOLUME) {                                            \
                                        return WRONG_ADDRESS;                                           \
                                    }                                                                   \
                                                                                                        \
                                    if (STR_EQ (#name, N_PUSH)) {                                  \
                                        val = softCPU->RAM[index];                                      \
                                                                                                        \
                                        PushStack (softCPU->st, val);                                   \
                                    }                                                                   \
                                    if (STR_EQ (#name, N_POP)) {                                   \
                                        val = PopStack (softCPU->st);                                   \
                                                                                                        \
                                        softCPU->RAM [index] = val;                                     \
                                    }                                                                   \
                                    continue;                                                           \
                                }                                                                       \
                                break;                                                                  \
                            }                                                                           \
                            case 2: {                                                                   \
                                code                                                                    \
                                break;                                                                  \
                            }                                                                           \
                            default : {                                                                 \
                                return UNKNOWN_COMMAND;                                                 \
                            }                                                                           \
                        }                                                                               \
                                                                                                        \
                        softCPU->ip++;                                                                  \
                        break;                                                                          \
                    }                                                                                   \


int RunCPU (CPU *softCPU) {
    assert (softCPU);

    double topVal = 0;
    double prevTopVal = 0;

    int index = 0;
    int shift = 0;
    double val = 0;

    while (1) {
        int cmd = *(softCPU->machineCode + softCPU->ip) & 0x1F;
        int type = *(unsigned char *)(softCPU->machineCode + softCPU->ip) & 0xE0;

        softCPU->ip++;

        int res = 0;

        switch (cmd) {
            #include "../Assembler/commands_def.h"

            #undef DEF_CMD_
        }   
    }
}