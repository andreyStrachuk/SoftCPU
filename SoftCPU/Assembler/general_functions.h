#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "commands.h"   

#define FindNextWord(src)               src = SkipSpaceSymbols (src); \
                                        if (*src == '\0' || *src == ';') continue \

#define FillBitField(field, a, command, type)     field.imm = 0; \
                                              field.reg = 0; \
                                              field.mem = 0; \
                                              if (type == IMM) { field.imm = 1;} \
                                              else if (type == REG) { field.reg = 1; } \
                                              else if (type == MEM) { field.mem = 1; } \
                                              field.cmd = command; \
                                              a = field.cmd + field.imm * (2 * 2 * 2 * 2 * 2 * 2) + field.reg * (2 * 2 * 2 * 2 * 2 * 2 * 2) + field.mem * (2 * 2 * 2 * 2 * 2 * 2 * 2 * 2)

#define ASSERT_CORRECT(cmds)            cmds[i]->str = SkipSpaceSymbols (cmds[i]->str); \
                                              if (*(cmds[i]->str) != ';' && *(cmds[i]->str) != '\0') { return INCORRECT_INPUT; }

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

int ReadCmdAndWrite (FILE *code, Line **cmds, int numbOfStrings);

void InitializeArrOfPointers (Line **cmds, char *src, int numberOfStrings);

Line *GetLine (char *src, Line *cmdLine);

#endif