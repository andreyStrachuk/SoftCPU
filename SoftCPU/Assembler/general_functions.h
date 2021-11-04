#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "commands.h"   

#define SIZEOFCODEARR 1000
#define NUMBEROFLABELS 100
#define EPSILON 1e-6

#define FindNextWord(src)               src = SkipSpaceSymbols (src); \
                                        if (*src == '\0' || *src == ';') continue \

#define FillBitField(field, a, command, type)     field.imm = 0; \
                                              field.reg = 0; \
                                              field.mem = 0; \
                                              if (type == IMM) { field.imm = 1;} \
                                              else if (type == REG) { field.reg = 1; } \
                                              else if (type == MEM) { field.mem = 1; } \
                                              field.cmd = command; \
                                              a = field.cmd + field.imm * (2 * 2 * 2 * 2 * 2) + field.reg * (2 * 2 * 2 * 2 * 2 * 2) + field.mem * (2 * 2 * 2 * 2 * 2 * 2 * 2)

#define ASSERT_CORRECT(src)            src = SkipSpaceSymbols (src); \
                                              if (*src != ';' && *src != '\0') { return INCORRECT_INPUT; }

#define CheckLabel(src, lbls, labelIp, sizeOfCodeArr)   int res = CheckIfLabel (src, lbls, labelIp, sizeOfCodeArr); \
                                                        printf ("labelIp = %d\ni = %d\n", res, *labelIp); \
                                                        if (res == OK) { \
                                                            lbls[*labelIp].ip = sizeOfCodeArr; \
                                                            (*labelIp)++; \
                                                            continue; \
                                                        }

struct Line {
    char *str;
    int length;
};

char *SkipSpaceSymbols (char *src);

char *SkipReadWord (char *src);

void PrintErrors (int typeOfError);

int GetFileSize (FILE *asmProgram);

int NumberOfStrings (FILE *asmProgram);

int DetectCommand (char *src);

int DetectRegister (char *src);

char *SkipArg (char *src);

int ReadCmdAndWrite (FILE *code, Line **cmds, int numbOfStrings,Label *lbls, int typeOfCheck, int *labelIp);

int PreAsmHandler (FILE *code, Line **cmds, int numbOfStrings);

void InitializeArrOfPointers (Line **cmds, char *src, int numberOfStrings);

void PutDouble (double value, char *ptr, int *sizeOfArr);

void PutInt (int value, char *ptr, int *sizeOfArr);

int CheckIfLabel (char *src, Label *lbls, int *labelIp, int sizeOfCodeArr);

Line *GetLine (char *src, Line *cmdLine);

void InitializeArrOfLabels (Label *lbls);

int FindLabelIndex (Label *lbls, char *src, int labelIp);

int CheckIfLabelContainsStr (Label *lbls, char *src, int labelIp);

#endif