#include "general_functions.h"
#include "commands.h"

#include <stdlib.h>

char *SkipSpaceSymbols (char *src) {
    assert (src);

    while (*src == ' ' && (*src != '\0' || *src != ';')) {
        src++;
    }

    return src;
}

char *SkipReadWord (char *src) {
    assert (src);

    while (*src != '\0' && *src != ' ' && *src != ';') {
        src++;
    }

    return src;
}

int GetFileSize (FILE *asmProgram) {
    assert (asmProgram);

    int numberOfSymbols = 0;

    fseek (asmProgram, 0, SEEK_END);

    numberOfSymbols = ftell (asmProgram);

    fseek (asmProgram, 0, SEEK_SET);

    return numberOfSymbols;
}

int NumberOfStrings (FILE *asmProgram) {
    assert (asmProgram);

    int number = 0;

    char symbol = (char)getc (asmProgram);

    while (symbol != EOF) {
        if (symbol == '\n') {
            number++;
        }

        symbol = (char)getc (asmProgram);
    }

    fseek (asmProgram, 0, SEEK_SET);

    return number;
}

int DetectCommand (char *src) { //!TODO strncmp
    assert (src); //!TODO my own assert with return
    int cmdNum = 0;

    if (strcmp (N_HLT, src) == 0) {
        cmdNum = HLT;
    } 
    
    else if (strcmp (N_MUL, src) == 0) {
        cmdNum = MUL;
    } 
    
    else if (strcmp (N_OUT, src) == 0) {
        cmdNum = OUT;
    } 
    
    else if (strcmp (N_PUSH, src) == 0) {
        cmdNum = PUSH;
    } 
    
    else if (strcmp (N_SUB, src) == 0) {
        cmdNum = SUB;
    } 
    
    else if (strcmp (N_ADD, src) == 0) {
        cmdNum = ADD;
    } 
    
    else if (strcmp (N_IN, src) == 0) {
        cmdNum = IN;
    } 
    
    else if (strcmp (N_SQRT, src) == 0) {
        cmdNum = SQRT;
    } 
    
    else if (strcmp (N_POP, src) == 0) {
        cmdNum = POP;
    }
    
    else if (strcmp (N_DIV, src) == 0) {
        cmdNum = DIV;
    }

    else if (strcmp (N_SIN, src) == 0) {
        cmdNum = SIN;
    }

    else if (strcmp (N_COS, src) == 0) {
        cmdNum = COS;
    }

    else {
        cmdNum = -10;
        return cmdNum;
    }

    return cmdNum;

}

int DetectRegister (char *src) {
    assert (src);

    int typeOfRegister = UNKNOWN_REGISTER;

    if (strcmp (N_AX, src) == 0) {
        typeOfRegister = AX;
    } else if (strcmp (N_BX, src) == 0) {
        typeOfRegister = BX;
    } else if (strcmp (N_CX, src) == 0) {
        typeOfRegister = CX;
    } else if (strcmp (N_DX, src) == 0) {
        typeOfRegister = DX;
    } else {
        typeOfRegister = UNKNOWN_REGISTER;
    }

    return typeOfRegister;
}

char *SkipArg (char *src) {
    assert (src);

    while (*src != ' ' && *src != '\n' && *src != '\0') src++; //!TODO isspace

    return src;
}

int ReadCmdAndWrite (FILE *code, Line **cmds, int numbOfStrings) {
    assert (code);
    assert (cmds);

    char commands [10] = {};
    char *machineCode = (char *)calloc (SIZEOFCODEARR, sizeof (char));
    int sizeOfCodeArr = 0;
    char reg [10] = {};
    unsigned char cmdNumber = 0;
    double arg = 0;
    struct COMMAND cmdNum = {};
    int typeOfReg = 0;
    int resOfScan = 0;
    int typeOfCmd = 0; 

    for (int i = 0; i < numbOfStrings; i++) {
        FindNextWord (cmds[i]->str);

        resOfScan = sscanf (cmds[i]->str, "%s", commands);

        typeOfCmd = DetectCommand (commands);
        if (typeOfCmd == UNKNOWN_COMMAND) {
            return UNKNOWN_COMMAND;
        }

        cmds[i]->str = SkipReadWord (cmds[i]->str);

        do {
            if (typeOfCmd == PUSH) {

                cmds[i]->str = SkipSpaceSymbols (cmds[i]->str); // skip arg skipspacesymbols
                if (*(cmds[i]->str) == ';' ||  *(cmds[i]->str) == '\0') {
                    return INCORRECT_INPUT;
                }

                resOfScan = sscanf (cmds[i]->str, "%lg", &arg);

                if (resOfScan != 0) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    FillBitField (cmdNum, cmdNumber, PUSH, IMM);

                    machineCode [sizeOfCodeArr++] = cmdNumber;

                    PutDouble (arg, machineCode, &sizeOfCodeArr);

                    break;
                }

                int shift = 0;
                int ok = 0;
                resOfScan = sscanf (cmds[i]->str, "[%d]%n", &shift, &ok);

                if (ok == 3) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    FillBitField (cmdNum, cmdNumber, PUSH, MEM);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    PutInt (shift, machineCode, &sizeOfCodeArr);

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "[%d+%2s]%n", &shift, reg, &ok);

                if (ok == 6) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    typeOfReg = DetectRegister (reg);
                    if (typeOfReg == UNKNOWN_REGISTER) {
                        return UNKNOWN_REGISTER;
                    }

                    FillBitField (cmdNum, cmdNumber, PUSH, MEM);
                    cmdNumber += (2 * 2 * 2 * 2 * 2 * 2);
                    cmdNumber += (2 * 2 * 2 * 2 * 2);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    machineCode[sizeOfCodeArr++] = typeOfReg;
                    PutInt (shift, machineCode, &sizeOfCodeArr);

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "[%2s]%n", reg, &ok);

                if (ok == 4) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    typeOfReg = DetectRegister (reg);
                    if (typeOfReg == UNKNOWN_REGISTER) {
                        return UNKNOWN_REGISTER;
                    }

                    FillBitField (cmdNum, cmdNumber, PUSH, MEM);
                    cmdNumber += (2 * 2 * 2 * 2 * 2 * 2);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    machineCode[sizeOfCodeArr++] = typeOfReg;

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "%s", reg);
                if (resOfScan == 0) {
                    return INCORRECT_INPUT;
                }
                    
                typeOfReg = DetectRegister (reg);
                if (typeOfReg == UNKNOWN_REGISTER) {
                    return UNKNOWN_REGISTER;
                }

                cmds[i]->str = SkipReadWord (cmds[i]->str);
                ASSERT_CORRECT (cmds);

                FillBitField (cmdNum, cmdNumber, PUSH, REG);

                machineCode [sizeOfCodeArr++] = cmdNumber;
                machineCode [sizeOfCodeArr++] = typeOfReg;

            }
            
            if (typeOfCmd == POP) {
                cmds[i]->str = SkipSpaceSymbols (cmds[i]->str);

                if (*(cmds[i]->str) == ';' || *(cmds[i]->str) == '\0') {
                    FillBitField (cmdNum, cmdNumber, POP, IMM);

                    machineCode [sizeOfCodeArr++] = cmdNumber;

                    break;
                }

                int shift = 0;
                int ok = 0;
                resOfScan = sscanf (cmds[i]->str, "[%d]%n", &shift, &ok);

                if (ok == 3) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    FillBitField (cmdNum, cmdNumber, POP, MEM);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    PutInt (shift, machineCode, &sizeOfCodeArr);

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "[%d+%2s]%n", &shift, reg, &ok);

                if (ok == 6) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    typeOfReg = DetectRegister (reg);
                    if (typeOfReg == UNKNOWN_REGISTER) {
                        return UNKNOWN_REGISTER;
                    }

                    FillBitField (cmdNum, cmdNumber, POP, MEM);
                    cmdNumber += (2 * 2 * 2 * 2 * 2 * 2);
                    cmdNumber += (2 * 2 * 2 * 2 * 2);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    machineCode[sizeOfCodeArr++] = typeOfReg;
                    PutInt (shift, machineCode, &sizeOfCodeArr);

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "[%2s]%n", reg, &ok);

                if (ok == 4) {
                    cmds[i]->str = SkipReadWord (cmds[i]->str);
                    ASSERT_CORRECT (cmds);

                    typeOfReg = DetectRegister (reg);
                    if (typeOfReg == UNKNOWN_REGISTER) {
                        return UNKNOWN_REGISTER;
                    }

                    FillBitField (cmdNum, cmdNumber, POP, MEM);
                    cmdNumber += (2 * 2 * 2 * 2 * 2 * 2);

                    machineCode[sizeOfCodeArr++] = cmdNumber;
                    machineCode[sizeOfCodeArr++] = typeOfReg;

                    break;
                }

                resOfScan = sscanf (cmds[i]->str, "%s", reg);
                typeOfReg = DetectRegister (reg);

                if (typeOfReg == UNKNOWN_REGISTER) {
                    return UNKNOWN_REGISTER;
                }

                cmds[i]->str = SkipReadWord (cmds[i]->str);
                ASSERT_CORRECT (cmds);

                FillBitField (cmdNum, cmdNumber, POP, REG);

                machineCode [sizeOfCodeArr++] = cmdNumber;
                machineCode [sizeOfCodeArr++] = typeOfReg;

                break;
            } 

        } while (0);

        if (typeOfCmd == IN) {
            cmds[i]->str = SkipReadWord (cmds[i]->str);
            ASSERT_CORRECT (cmds);

            printf ("Enter the number: ");

            int resOfSc = scanf ("%lg", &arg);

            if (resOfSc == 0) {
                return INCORRECT_INPUT;
            }

            FillBitField (cmdNum, cmdNumber, PUSH, IMM);

            machineCode [sizeOfCodeArr++] = cmdNumber;

            PutDouble (arg, machineCode, &sizeOfCodeArr);

            continue;
        }

        if (typeOfCmd != PUSH && typeOfCmd != POP) {
                ASSERT_CORRECT (cmds);

                FillBitField (cmdNum, cmdNumber, typeOfCmd, NTHG);
                machineCode[sizeOfCodeArr++] = cmdNumber;
        }

    }

    fwrite (machineCode, sizeof (char), sizeOfCodeArr, code);

    return OK;
}

Line *GetLine (char *src, Line *cmdLine) {
    assert (src);
    assert (cmdLine);

    cmdLine->str = src;

    while (*src != '\n') {
        cmdLine->length++;
        src++;
    }

    *src = '\0';
    cmdLine->length++;

    return cmdLine;
}

void InitializeArrOfPointers (Line **cmds, char *src, int numberOfStrings) {
    for (int i = 0; i < numberOfStrings; i++) {
        cmds [i] = (Line *) calloc (1, sizeof (Line *));
        assert (cmds [i]);

        cmds [i] = GetLine (src, cmds [i]);
        src += cmds [i]->length;
    }
}

void PrintErrors (int typeOfError) {
    switch (typeOfError) {
        case UNKNOWN_COMMAND : {
            printf ("Error!\nUnknown command!\n");
            break;
        }
        
        case UNKNOWN_REGISTER : {
            printf ("Error!\nUnknown register!\n");
            break;
        }

        case INCORRECT_INPUT : {
            printf ("Error!\nIncorrect input!\n");
            break;
        }
        case OK : {
            printf ("Everything is OK!\n");
            break;
        }

        default : {
            printf ("Unknown error!\nError number: %d\n", typeOfError);
        }
    }
}

void PutDouble (double value, char *ptr, int *sizeOfArr) {
    char *p = (char *)(&value);

    for (int i = 0; i < sizeof value; i++) {
        *(ptr + *sizeOfArr) = *p;
        p++;
        (*sizeOfArr)++;
    }
}

void PutInt (int value, char *ptr, int *sizeOfArr) {
    char *p = (char *)(&value);

    for (int i = 0; i < 2; i++) {
        *(ptr + *sizeOfArr) = *p;
        p++;
        (*sizeOfArr)++;
    }
}