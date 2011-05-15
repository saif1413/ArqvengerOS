#include "shell/echo/echo.h"
#include "library/stdio.h"
#include "library/string.h"
#include "mcurses/mcurses.h"

void echo(char* argv) {

    char* cmdEnd = strchr(argv, ' ');
    if (cmdEnd != NULL) {
        cmdEnd++;
        printf("%s\n", cmdEnd);
    }
}

void manEcho(void) {
    setBold(1);
    printf("echo\n");
    setBold(0);
    printf("\tPrint arguments to screen.\n\n");

    setBold(1);
    printf("Usage:\n\t echo");
    setBold(0);

    printf(" [string ...]\n");
}