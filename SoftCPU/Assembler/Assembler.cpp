#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "general_functions.h"
#include "commands.h"

int main () {

    FILE *asmProgFile = fopen ("program.asm", "rb");
    FILE *code = fopen ("code.bin", "wb");

    int fileSize = GetFileSize (asmProgFile);
    int numOfStrings = NumberOfStrings (asmProgFile);

    char *asmProg = (char *)calloc (fileSize, sizeof (char));
    fileSize = fread (asmProg, sizeof (char), fileSize, asmProgFile);

    Line **cmds = (Line **) calloc (numOfStrings, sizeof (Line *));

    InitializeArrOfPointers (cmds, asmProg, numOfStrings);
    int result = ReadCmdAndWrite (code, cmds, numOfStrings);

    if (result != OK) {
        PrintErrors (result);
    }

    free (asmProg);
    return 0;
}