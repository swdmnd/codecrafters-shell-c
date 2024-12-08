#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"
#include "echo.h"

int tokenize_cmd(char* input, char* output, int* len, int max_length);

int parse_cmd(char* cmd, FILE* out_stream){
    char cmd_token[MAX_CMD_LENGTH];
    int cmd_length = 0;
    int err = tokenize_cmd(cmd, cmd_token, &cmd_length, MAX_CMD_LENGTH);
    char* arg = cmd+cmd_length+1;
    if(err) {
        return err;
    }

    if(!strcmp(cmd_token, "exit")) {
        return EXIT_0;
    } else if(!strcmp(cmd_token, "echo")) {
        echo(arg, out_stream);
        return 0;
    } else if (!strcmp(cmd_token, "type")) {
        if(out_stream == NULL) {
            return 0;
        }
        int type_err = parse_cmd(arg, NULL);
        if(type_err == EXIT_0 || type_err == 0) {
            printf("%s is a shell builtin\n", arg);
            fflush(stdout);
            return 0;
        } else {
            printf("%s: not found\n", arg);
            fflush(stdout);
            return 0;
        }
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