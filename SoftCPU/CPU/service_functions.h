#ifndef SERVICE_H
#define SERVICE_H

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "processor.h"

int GetFileSize (FILE *code);

double GetRegValue (int reg, CPU *softCPU);

int WriteToRegister (int regType, CPU *softCPU, double val);

int RunCPU (CPU *softCPU);

#endif