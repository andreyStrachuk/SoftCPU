#ifndef SERVICE_H
#define SERVICE_H

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "processor.h"

#define RunProccessor(softCPU)      int res = RunCPU (softCPU);     \
                                    if (res != OK) {                \
                                        PrintError (res);       \
                                    }                               \

int GetFileSize (FILE *code);

double GetRegValue (int reg, CPU *softCPU);

int WriteToRegister (int regType, CPU *softCPU, double val);

int RunCPU (CPU *softCPU);

int CheckIfImm (const int type);

int CheckIfReg (const int type);

int CheckIfMem (const int type);

#endif