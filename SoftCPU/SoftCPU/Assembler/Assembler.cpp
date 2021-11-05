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
    int result = 0;

    char *asmProg = (char *)calloc (fileSize, sizeof (char));
    ASSERT_OK(asmProg == nullptr, PrintErrors (NULLPTR));

    fileSize = fread (asmProg, sizeof (char), fileSize, asmProgFile);
    fclose (asmProgFile);

    Line **cmds = (Line **)calloc (numOfStrings, sizeof (Line *));
    ASSERT_OK(cmds == nullptr, PrintErrors(NULLPTR)); 

    Label *lbls = (Label *)calloc (NUMBEROFLABELS, sizeof (Label));
    ASSERT_OK(lbls == nullptr, PrintErrors(NULLPTR));

    InitializeArrOfPointers (cmds, asmProg, numOfStrings);

    ReadCmdsAndWrite (code, cmds, numOfStrings, lbls, labelIp);

    MemFree (asmProg, cmds, numOfStrings, lbls, labelIp);

    fclose (code);
    
    return 0;
}