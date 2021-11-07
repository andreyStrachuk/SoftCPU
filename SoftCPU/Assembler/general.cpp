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

#define DEF_CMD_(name, number, argNumber, code, function)  if (STR_EQ (src, #name)) { \
                                    return CMD_##name; \
                                } \

int DetectCommand (char *src) { //!TODO strncmp
    assert (src); //!TODO my own assert with return

    #include "commands_def.h"

    #undef DEF_CMD_

    return UNKNOWN_COMMAND;
}

#define DEF_REG_(name, number)  if (STR_EQ (#name, src)) { \
                                    return R_##name; \
                                } \

int DetectRegister (char *src) {
    assert (src);

    #include "regs_def.h"

    #undef DEF_REG_

    return UNKNOWN_REGISTER;
}

char *SkipArg (char *src) {
    assert (src);

    while (*src != ' ' && *src != '\n' && *src != '\0') src++; //!TODO isspace

    return src;
}

#define DEF_CMD_(name, cmdNumb, argNumber, code, function)                                                                    \
            if (typeOfCmd == cmdNumb) {                                                                             \
                src = SkipSpaceSymbols (src);                                                                       \
                                                                                                                    \
                switch (argNumber) {                                                                                \
                    case 0: {                                                                                       \
                        if (*src == ';' ||  *src == '\0') {                                                         \
                                                                                                                    \
                            FillBitField (cmdNum, cmdNumber, typeOfCmd, NTHG);                                      \
                                                                                                                    \
                            machineCode [sizeOfCodeArr++] = cmdNumber;                                              \
                            continue;                                                                                  \
                        }                                                                                           \
                        else {                                                                                      \
                            return INCORRECT_INPUT;                                                                 \
                        }                                                                                           \
                        continue;                                                                                      \
                    }                                                                                               \
                                                                                                                    \
                    case 1: {                                                                                       \
                                                                                                                    \
                            function                                                                                \
                            continue;                                                                                  \
                    }                                                                                               \
                    case 2: {                                                                                       \
                        FindNextWord (src);                                                                         \
                        if (*src != ':') return INCORRECT_INPUT;                                                    \
                                                                                                                    \
                        function                                                                                    \
                                                                                                                    \
                        continue;                                                                                   \
                    }                                                                                               \
                    default : {                                                                                     \
                        return INCORRECT_INPUT;                                                                     \
                    }                                                                                               \
                }                                                                                                   \
            }


int ReadCmdAndWrite (FILE *code, Line **cmds, int numbOfStrings, Label *lbls, int typeOfCheck, int *labelIp) {
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
        char *src = cmds[i]->str;

        FindNextWord (src);

        int res = CheckIfLabel (src, lbls, labelIp, sizeOfCodeArr);
        if (res == OK) {
            continue;
        }

        resOfScan = sscanf (src, "%s", commands);
        typeOfCmd = DetectCommand (commands);

        if (typeOfCmd == UNKNOWN_COMMAND) {
            return UNKNOWN_COMMAND;
        }

        src = SkipReadWord (src);

        #include "commands_def.h"

        #undef DEF_CMD_

    }

    if (typeOfCheck == SECOND) {
        fwrite (machineCode, sizeof (char), sizeOfCodeArr, code);
    }

    free (machineCode);

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
            printf ("Unknown command!\nError code: %d\n", typeOfError);
            break;
        }
        
        case UNKNOWN_REGISTER : {
            printf ("Unknown register!\nError code: %d\n", typeOfError);
            break;
        }
        case ZERO_DIV: {
            printf ("Value is divided by ZERO!\nError code: %d\n", typeOfError);
            break;
        }  
        case WRONG_ADDRESS: {
            printf ("Invalid addres!\nError code: %d\n", typeOfError);
            break;
        }
        case INCORRECT_INPUT : {
            printf ("Incorrect input!\nError code: %d\n", typeOfError);
            break;
        }
        case UNABLETOREADFROMFILE : {
            printf ("Unable to read from file!\nError code: %d\n", typeOfError);
            break;
        }
        case NULLPTR : {
            printf ("Null pointer!\nError code: %d", typeOfError);
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

    for (int i = 0; i < (int)sizeof value; i++) {
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

int CheckIfLabel (char *src, Label *lbls, int *labelIp, int sizeOfCodeArr) {
    if (*src == ':') {
        src++;
        char str[10] = {0};
        int n = 0;

        sscanf (src, "%s%n", str, &n);

        int res = CheckIfLabelContainsStr (lbls, str, *labelIp);
        if (res != -1) {
            return OK;
        }

        (lbls + *labelIp)->txt = (char *)calloc (n, sizeof (char)); 

        strncpy ((*(lbls + *labelIp)).txt, str, n);

        (*(lbls + *labelIp)).ip = sizeOfCodeArr;

        (*labelIp)++;

        return OK;
    }

    return 0;
}

int CheckIfLabelContainsStr (Label *lbls, char *src, int labelIp) {
    assert (lbls);
    assert (src);

    int dest = -1;

    for (int i = 0; i < labelIp; i++) {
        if (STR_EQ (lbls[i].txt, src)) {
            dest = i;
            break;
        } 
    }

    return dest;
}

void MemFree (char *asmProg, Line **cmds, int numOfStrings, Label *lbls, int labelIp) {
    assert (asmProg);
    assert (cmds);
    assert (lbls);
    
    for (int i = 0; i < numOfStrings; i++) {
        free (cmds[i]);
    }

    for (int i = 0; i < labelIp; i++) {
        free (lbls[i].txt);
    }

    free (cmds);
    free (lbls);
}

int ArrangeCmd (char *src, int typeOfCmd, char *machineCode, int *sizeOfCodeArr) {
  
    double arg = 0;
    int resOfScan = sscanf (src, "%lg", &arg); 
    struct COMMAND field = {};    
    unsigned char cmdNumber = 0;       
    char reg[5] = {};
    int typeOfReg = 0;

    if (typeOfCmd == CMD_pop && (*src == ';' ||  *src == '\0')) {
        FillBitField (field, cmdNumber, typeOfCmd, IMM);
        machineCode [(*sizeOfCodeArr)++] = cmdNumber;
        return OK;
    }                           
                                                                                                                    
    if (resOfScan != 0) {                                                                   
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                                                    
        FillBitField (field, cmdNumber, typeOfCmd, IMM);                                   
                                                                                                                    
        machineCode [(*sizeOfCodeArr)++] = cmdNumber;                                          
                                                                                                                    
        PutDouble (arg, machineCode, sizeOfCodeArr);

        return OK;                                                                   
    }

    int shift = 0;
    int ok = 0;
    resOfScan = sscanf (src, "[%d]%n", &shift, &ok);                         
                                                                                                    
    if (ok >= 3) {                                                                          
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                            
        FillBitField (field, cmdNumber, typeOfCmd, MEM);                                  
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                          
        PutInt (shift, machineCode, sizeOfCodeArr);    

        return OK;                                                                     
    }

    resOfScan = sscanf (src, "[%d+%2s]%n", &shift, reg, &ok);                               
                                                                                                                    
    if (ok >= 6) {                                                                          
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                            
        DetectReg (reg);                                                                    
                                                                                            
        FillBitField (field, cmdNumber, typeOfCmd, MEMREGIMM);                             
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                        
        machineCode[(*sizeOfCodeArr)++] = typeOfReg;                                        
        PutInt (shift, machineCode, sizeOfCodeArr);                                        
                                                                                           
        return OK;                                                                         
    }

    resOfScan = sscanf (src, "[%s]%n", reg, &ok);                                          
                                                                                                                    
    if (ok == 4) {                                                                          
        src = SkipReadWord (src);                                                           
        ASSERT_CORRECT (src);                                                               
                                                                                            
        DetectReg (reg);                                                                  
                                                                                        
        FillBitField (field, cmdNumber, typeOfCmd, MEMREG);                                
                                                                                            
        machineCode[(*sizeOfCodeArr)++] = cmdNumber;                                           
        machineCode[(*sizeOfCodeArr)++] = typeOfReg;                                           
                                                                                            
        return OK;                                                                           
    }

    resOfScan = sscanf (src, "%2s", reg);                                                   
    ASSERT_OKAY (resOfScan == 0, return UNKNOWN_REGISTER;)                                                                                       
                                                                                            
    DetectReg (reg);                                                                        
                                                                                            
    src = SkipReadWord (src);                                                               
    ASSERT_CORRECT (src);                                                                   
                                                                                            
    FillBitField (field, cmdNumber, typeOfCmd, REG);                                       
                                                                                            
    machineCode [(*sizeOfCodeArr)++] = cmdNumber;                                              
    machineCode [(*sizeOfCodeArr)++] = typeOfReg;                                              
    return OK;

}

int ArrangeJmpsCall (char *src, int typeOfCmd, char *machineCode, int *sizeOfCodeArr, Label *lbls, int *labelIp) {
    int dest = -1;
    char commands [10] = {};

    src++;                                                                                            
    int resOfScan = sscanf (src, "%s", commands);                                                                            
                                                                                                
    for (int i = 0; i < *labelIp; i++) {                                                        
        if (strcmp (lbls[i].txt, commands) == 0) {                                              
            dest = i;                                                                           
            break;                                                                              
        }                                                                                       
    }                                                                                           
    machineCode [(*sizeOfCodeArr)++] = typeOfCmd;                                                  
    if (dest != -1) {                                                                                                                                                                
        PutInt (lbls[dest].ip, machineCode, sizeOfCodeArr);                                    
    }                                                                                           
    else {                                                                                      
        PutInt (-1, machineCode, sizeOfCodeArr);                                               
    }

    return OK;
}