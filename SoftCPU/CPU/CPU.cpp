#include "../lib/stack.h"
#include "../lib/verificator.h"
#include "../lib/dump.h"
#include "../lib/unit_tests.h"
#include "../Assembler/commands.h"
#include "processor.h"

#include "service_functions.h"

int main () {
    CPU softCPU = {};

    softCPU.st = (Stack *)calloc (1, sizeof (Stack));

    InitStack (softCPU.st);

    COMMAND cmd = {};

    FILE *code = fopen ("../Assembler/code.bin", "rb");
    assert (code);

    int fileSize = GetFileSize (code);

    softCPU.machineCode = (char *)calloc (fileSize, sizeof (char));

    fread (softCPU.machineCode, sizeof (char), fileSize, code);

    int res = RunCPU (&softCPU);

    return 0;
}