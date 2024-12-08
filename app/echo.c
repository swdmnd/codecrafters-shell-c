#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"
#include "echo.h"

void echo(char* str, FILE* out_stream) {
    if(out_stream != NULL) {
        printf("%s\n", str);
        fflush(out_stream);
    }
}