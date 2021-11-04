#include "service_functions.h"
#include "../Assembler/commands.h"
#include "processor.h"

double GetRegValue (int reg, CPU *softCPU) {
    switch (reg)
    {
    case R_AX:
        return softCPU->ax;
    
    case R_BX: 
        return softCPU->bx;

    case R_CX:
        return softCPU->cx;

    case R_DX: 
        return softCPU->dx;
    
    default:
        return UNKNOWN_REGISTER;
    }
}

int WriteToRegister (int regType, CPU *softCPU, double val) {
    assert (softCPU);

    switch (regType) {
        case R_AX:
            softCPU->ax = val;
            break;

        case R_BX:
            softCPU->bx = val;
            break;

        case R_CX:
            softCPU->cx = val;
            break;

        case R_DX:
            softCPU->dx = val;
            break;

        default : 
            return UNKNOWN_REGISTER;
    }

    return OK;

}

#define DEF_CMD_(name, cmdNumb, argNumber, code)                                                       \
                    case CMD_##name: {                                                                 \
                        if (strcmp (#name, "push") == 0) {                                          \
                            if (type & 0x20 && !(type & 0x40) && !(type & 0x80)) {                     \
                                                                                                       \
                                val = *(double *)(softCPU->machineCode + softCPU->ip);                 \
                                                                                                       \
                                code                                                                   \
                                                                                                       \
                                softCPU->ip += sizeof (double);                                        \
                            }                                                                          \
                                                                                                       \
                            else if (type & 0x40 && type & 0x80 == 0) {                                \
                                                                                                       \
                                                                                                       \
                                int reg = *(char *)(softCPU->machineCode + softCPU->ip);               \
                                                                                                       \
                                double valueOfReg = GetRegValue (reg, softCPU);                        \
                                                                                                       \
                                PushStack (softCPU->st, valueOfReg);                                   \
                                                                                                       \
                                softCPU->ip++;                                                         \
                            }                                                                          \
                                                                                                       \
                            else if (type & 0x80 && type & 0x40 == 0 && type & 0x20 == 0) {            \
                                                                                                       \
                                softCPU->ip++;                                                         \
                                                                                                       \
                                shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);            \
                                                                                                       \
                                softCPU->ip += 2;                                                      \
                                                                                                       \
                                if (shift > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = softCPU->RAM [shift];                                     \
                                                                                                       \
                                PushStack (softCPU->st, val);                                          \
                                                                                                       \
                            }                                                                          \
                                                                                                       \
                            else if (type & 0x80 && type & 0x40 && type & 0x20 == 0) {                 \
                                                                                                       \
                                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);         \
                                                                                                       \
                                softCPU->ip++;                                                         \
                                                                                                       \
                                int index = GetRegValue (typeOfReg, softCPU);                          \
                                                                                                       \
                                if (index > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = softCPU->RAM [index];                                     \
                                                                                                       \
                                PushStack (softCPU->st, val);                                          \
                                                                                                       \
                            }                                                                          \
                                                                                                       \
                            if (type & 0x80 && type & 0x40 && type & 0x20) {                           \
                                                                                                       \
                                int typeOfReg = *(softCPU->machineCode + softCPU->ip);                 \
                                                                                                       \
                                softCPU->ip ++;                                                        \
                                                                                                       \
                                double regValue = GetRegValue (typeOfReg, softCPU);                    \
                                                                                                       \
                                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);        \
                                softCPU->ip += 2;                                                      \
                                                                                                       \
                                int index = (int)regValue + shift;                                     \
                                                                                                       \
                                if (index > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = softCPU->RAM[index];                                      \
                                                                                                       \
                                PushStack (softCPU->st, val);                                          \
                                                                                                       \
                            }                                                                          \
                                                                                                       \
                            break;                                                                     \
                                                                                                       \
                        }                                                                              \
                        else if (strcmp (#name, "pop") == 0) {                                      \
                            if ((type & 0x20) && !(type & 0x40) && !(type & 0x80)) {                   \
                                                                                                       \
                                PopStack (softCPU->st);                                                \
                                                                                                       \
                                break;                                                                 \
                            }                                                                          \
                                                                                                       \
                            if (type & 0x40 && !(type & 0x80) && !(type & 0x40)) {                     \
                                                                                                       \
                                int reg = *(char *)(softCPU->machineCode + softCPU->ip);               \
                                                                                                       \
                                double val = PopStack (softCPU->st);                                   \
                                                                                                       \
                                int res = WriteToRegister (reg, softCPU, val);                         \
                                                                                                       \
                                if (res == UNKNOWN_REGISTER) {                                         \
                                    return UNKNOWN_REGISTER;                                           \
                                }                                                                      \
                                                                                                       \
                                softCPU->ip++;                                                         \
                                break;                                                                 \
                            }                                                                          \
                                                                                                       \
                            if (type & 0x80 && !(type & 0x40)) {                                       \
                                                                                                       \
                                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);        \
                                                                                                       \
                                softCPU->ip += 2;                                                      \
                                                                                                       \
                                if (shift > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = PopStack (softCPU->st);                                   \
                                                                                                       \
                                softCPU->RAM [shift] = val;                                            \
                                                                                                       \
                            }                                                                          \
                                                                                                       \
                            if ((type & 0x80) && (type & 0x40) && !(type & 0x20)) {                    \
                                softCPU->ip++;                                                         \
                                                                                                       \
                                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);         \
                                                                                                       \
                                softCPU->ip++;                                                         \
                                                                                                       \
                                int index = GetRegValue (typeOfReg, softCPU);                          \
                                                                                                       \
                                if (index > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = PopStack (softCPU->st);                                   \
                                                                                                       \
                                softCPU->RAM [index] = val;                                            \
                            }                                                                          \
                                                                                                       \
                            if (type & 0x80 && type & 0x40 && type & 0x20) {                           \
                                softCPU->ip++;                                                         \
                                                                                                       \
                                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);         \
                                if (typeOfReg == UNKNOWN_REGISTER) {                                   \
                                    return UNKNOWN_REGISTER;                                           \
                                }                                                                      \
                                                                                                       \
                                softCPU->ip ++;                                                        \
                                                                                                       \
                                int regValue = GetRegValue (typeOfReg, softCPU);                       \
                                                                                                       \
                                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);        \
                                softCPU->ip += 2;                                                      \
                                                                                                       \
                                int index = (int)regValue + shift;                                     \
                                                                                                       \
                                if (index > RAMVOLUME) {                                               \
                                    return WRONG_ADDRESS;                                              \
                                }                                                                      \
                                                                                                       \
                                double val = PopStack (softCPU->st);                                   \
                                                                                                       \
                                softCPU->RAM [index] = val;                                            \
                            }                                                                          \
                            break;                                                                     \
                        }                                                                              \
                        else {                                                                         \
                            code                                                                       \
                        }                                                                              \
                                                                                                       \
                        softCPU->ip++;                                                                 \
                        break;                                                                         \
                    }                                                                                  \
                                                                                                       \


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

        switch (cmd) {
            #include "../Assembler/commands_def.h"

            #undef DEF_CMD_
        }
        
    }

    return OK;

}