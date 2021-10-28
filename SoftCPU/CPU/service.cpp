#include "service_functions.h"
#include "../Assembler/commands.h"
#include "processor.h"

int GetFileSize (FILE *code) {
    assert (code);

    int numberOfSymbols = 0;

    fseek (code, 0, SEEK_END);

    numberOfSymbols = ftell (code);

    fseek (code, 0, SEEK_SET);

    return numberOfSymbols;
}

double GetRegValue (int reg, CPU *softCPU) {
    switch (reg)
    {
    case AX:
        return softCPU->ax;
    
    case BX: 
        return softCPU->bx;

    case CX:
        return softCPU->cx;

    case DX: 
        return softCPU->dx;
    
    default:
        return UNKNOWN_REGISTER;
    }
}

int WriteToRegister (int regType, CPU *softCPU, double val) {
    assert (softCPU);

    switch (regType) {
    case AX:
        softCPU->ax = val;
        break;

    case BX:
        softCPU->bx = val;
        break;

    case CX:
        softCPU->cx = val;
        break;

    case DX:
        softCPU->dx = val;
        break;

    default : 
        return UNKNOWN_REGISTER;
    }

    return OK;

}

int RunCPU (CPU *softCPU) {
    assert (softCPU);

    double topVal = 0;
    double prevTopVal = 0;

    while (1) {
        int cmd = *(softCPU->machineCode + softCPU->ip) & 0x1F;
        int type = *(softCPU->machineCode + softCPU->ip) & 0xE0;

        switch (cmd) {
        case PUSH:
            if (type & 0x40) {
                softCPU->ip++;
                double val = *(double *)(softCPU->machineCode + softCPU->ip);

                PushStack (softCPU->st, val);

                softCPU->ip += sizeof (double);
            }

            if (type & 0x80) {
                softCPU->ip++;

                int reg = *(char *)(softCPU->machineCode + softCPU->ip);

                double valueOfReg = GetRegValue (reg, softCPU);

                PushStack (softCPU->st, valueOfReg);

                softCPU->ip++;
            }
            break;

        case POP: 
            if (type & 0x40) {
                softCPU->ip++;
                
                PopStack (softCPU->st);
                
                softCPU->ip++;
            }

            if (type & 0x80) {
                softCPU->ip++;

                int reg = *(char *)(softCPU->machineCode + softCPU->ip);

                double val = PopStack (softCPU->st); 

                int res = WriteToRegister (reg, softCPU, val);

                if (res == UNKNOWN_REGISTER) {
                    return UNKNOWN_REGISTER;
                }

                softCPU->ip++;
            }
            break; 

        case ADD: 
            topVal = PopStack (softCPU->st);
            prevTopVal = PopStack (softCPU->st);

            PushStack (softCPU->st, topVal + prevTopVal);

            softCPU->ip++;

            break;

        case SUB:
            topVal = PopStack (softCPU->st);
            prevTopVal = PopStack (softCPU->st);

            PushStack (softCPU->st, topVal - prevTopVal);

            softCPU->ip++;

            break;        

        case MUL:
            topVal = PopStack (softCPU->st);
            prevTopVal = PopStack (softCPU->st);

            PushStack (softCPU->st, topVal * prevTopVal);

            softCPU->ip++;

            break; 

        case DIV:
            topVal = PopStack (softCPU->st);
            prevTopVal = PopStack (softCPU->st);

            if (prevTopVal != 0) {

                PushStack (softCPU->st, topVal / prevTopVal);

                softCPU->ip++;
            } else {
                return ZERO_DIV;
            }

            break;

        case SQRT:
            topVal = PopStack (softCPU->st);

            PushStack (softCPU->st, sqrt (topVal));

            softCPU->ip++;

            break; 

        case OUT: 
            printf ("Top value of CPU stack: %lg\n", PopStack (softCPU->st));

            softCPU->ip++;

            break;

        case HLT: 
            printf ("Program has been finished!\n");
            return HLT;

        
        default:
            break;
        }
        
    }

    return OK;

}