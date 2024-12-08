#include <string.h>
#include "cmd_parser.h"

int parse_cmd(char* cmd){
    if(!strcmp(cmd, "exit 0")) {
        return EXIT_0;
    }
    return ERR_INVALID_CMD;
}