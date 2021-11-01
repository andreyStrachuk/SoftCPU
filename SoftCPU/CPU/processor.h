#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "../lib/stack.h"

#define RAMVOLUME 16384

struct CPU {
    Stack *st;

    double ax;
    double bx;
    double cx;
    double dx;

    char *machineCode;
    double *RAM;

    int ip;
};

#endif