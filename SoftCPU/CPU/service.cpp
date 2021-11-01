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

    int index = 0;
    int shift = 0;
    double val = 0;

    while (1) {
        int cmd = *(softCPU->machineCode + softCPU->ip) & 0x1F;
        int type = *(unsigned char *)(softCPU->machineCode + softCPU->ip) & 0xE0;

        switch (cmd) {
        case PUSH:
            if (type & 0x20 && !(type & 0x40) && !(type & 0x80)) { // ob binary code
                printf ("1 push\n");
                softCPU->ip++;
                val = *(double *)(softCPU->machineCode + softCPU->ip);

                PushStack (softCPU->st, val);

                softCPU->ip += sizeof (double);
            }

            else if (type & 0x40 && type & 0x80 == 0) {
                printf ("2 push\n");
                softCPU->ip++;

                int reg = *(char *)(softCPU->machineCode + softCPU->ip);

                double valueOfReg = GetRegValue (reg, softCPU);

                PushStack (softCPU->st, valueOfReg);

                softCPU->ip++;
            }

            else if (type & 0x80 && type & 0x40 == 0 && type & 0x20 == 0) {

                printf ("3 push\n");

                softCPU->ip++;

                shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);

                softCPU->ip += 2;

                if (shift > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = softCPU->RAM [shift];

                PushStack (softCPU->st, val);

            }

            else if (type & 0x80 && type & 0x40 && type & 0x20 == 0) {

                printf ("4 push\n");

                softCPU->ip++;

                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);

                softCPU->ip++;

                int index = GetRegValue (typeOfReg, softCPU);

                if (index > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = softCPU->RAM [index];

                PushStack (softCPU->st, val);

            }

            if (type & 0x80 && type & 0x40 && type & 0x20) {
                softCPU->ip++;
                printf ("5 push\n");

                int typeOfReg = *(softCPU->machineCode + softCPU->ip);

                printf ("typeofreg = %d\n", typeOfReg);

                softCPU->ip ++;

                double regValue = GetRegValue (typeOfReg, softCPU);

                printf ("reg value = %lg\n", regValue);

                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);
                softCPU->ip += 2;

                printf ("shift = %d\n", shift);

                int index = (int)regValue + shift; 

                if (index > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = softCPU->RAM[index];

                PushStack (softCPU->st, val);      
            }
            break;

        case POP: 
            if ((type & 0x20) && !(type & 0x40) && !(type & 0x80)) {
                softCPU->ip++;
                printf ("1 pop\n");
                
                PopStack (softCPU->st);
                
                break;
            }

            if (type & 0x40 && !(type & 0x80) && !(type & 0x40)) {
                printf ("2 pop\n");
                softCPU->ip++;

                int reg = *(char *)(softCPU->machineCode + softCPU->ip);

                double val = PopStack (softCPU->st); // define PopStack

                int res = WriteToRegister (reg, softCPU, val);

                if (res == UNKNOWN_REGISTER) {
                    return UNKNOWN_REGISTER;
                }

                softCPU->ip++;
                break;
            }

            if (type & 0x80 && !(type & 0x40)) {
                printf ("3 pop\n");
                softCPU->ip++;

                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);

                softCPU->ip += 2;

                if (shift > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = PopStack (softCPU->st);

                softCPU->RAM [shift] = val;

            }

            if ((type & 0x80) && (type & 0x40) && !(type & 0x20)) {
                printf ("4 pop\n");
                softCPU->ip++;

                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);

                softCPU->ip++;

                int index = GetRegValue (typeOfReg, softCPU);

                if (index > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = PopStack (softCPU->st);

                softCPU->RAM [index] = val;           
            }

            if (type & 0x80 && type & 0x40 && type & 0x20) {
                printf ("5 pop\n");
                softCPU->ip++;

                int typeOfReg = *(char *)(softCPU->machineCode + softCPU->ip);
                if (typeOfReg == UNKNOWN_REGISTER) {
                    return UNKNOWN_REGISTER;
                }

                softCPU->ip ++;

                int regValue = GetRegValue (typeOfReg, softCPU);

                int shift = *(u_int16_t *)(softCPU->machineCode + softCPU->ip);
                softCPU->ip += 2;

                int index = (int)regValue + shift; 

                if (index > RAMVOLUME) {
                    return WRONG_ADDRESS;
                }

                double val = PopStack (softCPU->st);

                softCPU->RAM [index] = val;           
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

        case SIN:
            topVal = PopStack (softCPU->st);

            PushStack (softCPU->st, sin (topVal));

            softCPU->ip++;

            break; 

        case COS:
            topVal = PopStack (softCPU->st);

            PushStack (softCPU->st, cos (topVal));

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