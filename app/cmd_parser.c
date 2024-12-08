#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"
#include "echo.h"

int tokenize_cmd(char* input, char* output, int* len, int max_length);

int parse_cmd(char* cmd){
    if(!strcmp(cmd, "exit 0")) {
        return EXIT_0;
    }
    char cmd_token[MAX_CMD_LENGTH];
    int cmd_length = 0;
    int err = tokenize_cmd(cmd, cmd_token, &cmd_length, MAX_CMD_LENGTH);
    if(err) {
        return err;
    }

    if(!strcmp(cmd_token, "echo")){
        char* str_to_print = cmd+cmd_length+1;
        echo(str_to_print);
        return 0;
    }

    return ERR_INVALID_CMD;
}

int tokenize_cmd(char* input, char* output, int* len, int max_length){
    *len = strcspn(input, " ");
    if(*len >= max_length){
        return ERR_CMD_LENGTH_LIMIT;
    }
    memcpy(output, input, *len);
    output[*len] = 0;
    return 0;
}