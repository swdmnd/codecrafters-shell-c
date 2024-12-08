#include "echo.h"
#include <stdio.h>
#include <string.h>

void echo(char* input) {
    printf("%s\n", input);
    fflush(stdout);
}