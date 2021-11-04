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
    int labelIp = 0;

    char *asmProg = (char *)calloc (fileSize, sizeof (char));
    fileSize = fread (asmProg, sizeof (char), fileSize, asmProgFile);

    Line **cmds = (Line **) calloc (numOfStrings, sizeof (Line *));

    Label *lbls = (Label *)calloc (NUMBEROFLABELS, sizeof (Label));

    InitializeArrOfLabels (lbls);

    InitializeArrOfPointers (cmds, asmProg, numOfStrings);
    int result = ReadCmdAndWrite (code, cmds, numOfStrings, lbls, FIRST, &labelIp);

    if (result != OK) {
        PrintErrors (result);
    }

    result = ReadCmdAndWrite (code, cmds, numOfStrings, lbls, SECOND, &labelIp);

    if (result != OK) {
        PrintErrors (result);
    }

    for (int i = 0; i < labelIp; i++) {
        printf ("string = %s\nip = %d\n", lbls[i].txt, lbls[i].ip);
    }

    printf ("label ip = %d\n", labelIp);

    free (asmProg);
    return 0;
}