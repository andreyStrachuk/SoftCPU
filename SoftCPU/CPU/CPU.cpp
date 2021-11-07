#include "../lib/stack.h"
#include "../lib/verificator.h"
#include "../lib/dump.h"
#include "../lib/unit_tests.h"
#include "../Assembler/commands.h"
#include "../Assembler/general_functions.h"
#include "processor.h"

#include "service_functions.h"

int main () {
    CPU softCPU = {};

    softCPU.st = (Stack *)calloc (1, sizeof (Stack));
    softCPU.call = (Stack *)calloc (1, sizeof (Stack));

    InitStack (softCPU.st);
    InitStack (softCPU.call);

    FILE *code = fopen ("Assembler/code.bin", "rb");
    ASSERT_OKAY (code == nullptr, PrintErrors (NULLPTR));

    int fileSize = GetFileSize (code);

    softCPU.machineCode = (char *)calloc (fileSize, sizeof (char));
    ASSERT_OKAY (softCPU.machineCode == nullptr, PrintErrors (NULLPTR));

    softCPU.RAM = (double *)calloc (RAMVOLUME, sizeof (double));
    ASSERT_OKAY (softCPU.RAM == nullptr, PrintErrors (NULLPTR));

    int numberOfReadSym = fread (softCPU.machineCode, sizeof (char), fileSize, code);
    ASSERT_OKAY (numberOfReadSym == 0, PrintErrors (UNABLETOREADFROMFILE));

    RunProccessor (&softCPU);

    DestructStack (softCPU.st);
    DestructStack (softCPU.call);

    free (softCPU.st);
    free (softCPU.call);
    free (softCPU.machineCode);
    free (softCPU.RAM);

    return 0;
}