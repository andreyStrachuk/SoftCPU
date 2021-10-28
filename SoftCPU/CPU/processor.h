#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "../lib/stack.h"

struct CPU {
    Stack *st;

    double ax;
    double bx;
    double cx;
    double dx;

    char *machineCode;

    int ip;
};

#endif