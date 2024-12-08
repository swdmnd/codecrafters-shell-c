#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "shell.h"
#include "echo.h"

#define ERR_TOKENIZE 0x10

int tokenize_first(char* input, char* output, int* len, int max_length, char* delimiter);
int is_executable(char* path, char* filename);

int parse_cmd(char* cmd, FILE* out_stream){
    char cmd_token[MAX_CMD_LENGTH];
    int cmd_length = 0;
    int err = tokenize_first(cmd, cmd_token, &cmd_length, MAX_CMD_LENGTH, " ");
    char* arg = cmd+cmd_length+1;
    if(err == ERR_TOKEN_LENGTH_LIMIT) {
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
            char* paths = ENV_PATH;
            int path_len;
            char cur_path[256];
            int dir_err = 0;
            int file_err = 0;
            do{
                dir_err = tokenize_first(paths, cur_path, &path_len, 256, ":");
                if(dir_err != ERR_TOKENIZE) {
                    paths = paths + path_len + 1;
                } else {
                    strcpy(cur_path, paths);
                }
                fflush(stdout);
                file_err = is_executable(cur_path, arg);
                if(!file_err) {
                    printf("%s is %s/%s\n", arg, cur_path, arg);
                    fflush(stdout);
                    return 0;
                }
            } while(dir_err != ERR_TOKENIZE);
            printf("%s: not found\n", arg);
            fflush(stdout);
            return 0;
        }
    }

    return ERR_INVALID_CMD;
}

int tokenize_first(char* input, char* output, int* len, int max_length, char* delimiter){
    *len = strcspn(input, delimiter);
    if(*len >= max_length){
        return ERR_TOKEN_LENGTH_LIMIT;
    }

    memcpy(output, input, *len);
    output[*len] = 0;

    if(*len == strlen(input)) {
        return ERR_TOKENIZE;
    }
    
    return 0;
}

int is_executable(char* path, char* filename) {
    DIR* d;
    struct dirent* dir;
    d = opendir(path);
    bool is_found = false;
    if(d) {
        while ((dir = readdir(d)) != NULL) {
            if(!strcmp(dir->d_name, filename)){
                is_found = true;
                break;
            }
        }
        closedir(d);
    }
    if(is_found){
        return 0;
    }
    return ERR_FILE_NOT_FOUND;
}