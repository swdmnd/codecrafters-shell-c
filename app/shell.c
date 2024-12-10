#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "shell.h"
#include "echo.h"

#define ERR_TOKENIZE 0x10
#define TYPE_EXECUTABLE 0x20

typedef struct {
    char* cmd;
    char* argv[100];
    int argc;
    char final_cmd[1000];
} Command;

Command __command;

int tokenize_first(char* input, char* output, int* len, int max_length, char* delimiter);
int tokenize(char* input, Command* cmd);
int is_executable(char* path, char* filename);

int parse_cmd(char* cmd, FILE* out_stream){
    if(strlen(cmd) == 0){
        return 0;
    }

    tokenize(cmd, &__command);

    if(!strcmp(__command.cmd, "exit")) {
        return EXIT_0;
    } else if(!strcmp(__command.cmd, "echo")) {
        for(int i = 0; i < __command.argc; ++i){
            printf("%s", __command.argv[i]);
            if(i != __command.argc - 1){
                printf(" ");
            } else {
                printf("\n");
            }
            fflush(stdout);
        }
        return 0;
    } else if(!strcmp(__command.cmd, "pwd")) {
        char workdir[1024];
        getcwd(workdir, 1024);
        echo(workdir, out_stream);
        return 0;
    } else if(!strcmp(__command.cmd, "cd")) {
        char cdpath[1024];
        if(__command.argc > 0){
            if(__command.argv[0][0] == '~'){
                sprintf(cdpath, "%s%s", getenv("HOME"), __command.argv[0]+1);
            } else {
                sprintf(cdpath, "%s", __command.argv[0]);
            }
            int cd_err = chdir(cdpath);
            if(cd_err) {
                printf("cd: %s: No such file or directory\n", __command.argv[0]);
                fflush(out_stream);
            }
        }
        
        return 0;
    } else if (!strcmp(__command.cmd, "type")) {
        if(out_stream == NULL || __command.argc == 0) {
            return 0;
        }
        int type_err = parse_cmd(__command.argv[0], NULL);
        if(type_err == EXIT_0 || type_err == 0) {
            printf("%s is a shell builtin\n", __command.argv[0]);
            fflush(stdout);
            return 0;
        } else if (type_err == TYPE_EXECUTABLE) {
            return 0;
        } else if (type_err == ERR_INVALID_CMD) {
            printf("%s: not found\n", __command.argv[0]);
            fflush(stdout);
            return 0;
        }
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
            file_err = is_executable(cur_path, __command.cmd);
            if(!file_err) {
                if(out_stream == NULL) {
                    printf("%s is %s/%s\n", __command.cmd, cur_path, __command.cmd);
                    fflush(stdout);
                    return TYPE_EXECUTABLE;
                }
                // OVERFLOW!!
                char execmd[2048];
                sprintf(execmd, "%s/%s", cur_path, __command.final_cmd);
                system(execmd);
                return 0;
            }
        } while(dir_err != ERR_TOKENIZE);
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

int tokenize(char* input, Command* cmd) {
    // clear cmd
    cmd->cmd = NULL;
    // clear final
    cmd->final_cmd[0] = '\0';
    int input_length = strlen(input);
    char* token = input;
    int token_ctr = 0;
    int argc = 0;
    char encountered_quote = 0;
    bool is_escaping = false;
    char tmp_quote[2] = {0,0};
    for(int i = 0; i < input_length && input[i] != 0; ++i){
        if(encountered_quote == 0 && input[i] == '\\') {
            //shift
            for(int j = i; input[j] != '\0'; ++j){
                input[j] = input[j+1];
            }
            continue;
        }

        if(encountered_quote == 0 && (input[i] == '\'' || input[i] == '"')) {
            encountered_quote = input[i];
            ++token;
            continue;
        }
        if(
            (encountered_quote == 0 && input[i] == ' ') || 
            (encountered_quote != 0 && input[i] == encountered_quote)
          ) {
                tmp_quote[0] = encountered_quote;
                encountered_quote = 0;
                if(token_ctr == 0){
                    ++token;
                    continue;
                }

                input[i] =  '\0';
                if(cmd->cmd == NULL) {
                    cmd->cmd = token;
                } else {
                    cmd->argv[argc++] = token;
                    strcat(cmd->final_cmd," ");
                }
                
                strcat(cmd->final_cmd, tmp_quote);
                strcat(cmd->final_cmd, token);
                strcat(cmd->final_cmd, tmp_quote);
                    
                token = token + token_ctr + 1;
                token_ctr = 0;
                continue;
        }
        
        ++token_ctr;
    }

    if(token_ctr != 0 && token != 0){
        if(cmd->cmd == NULL) {
            cmd->cmd = token;
        } else {
            cmd->argv[argc++] = token;
            strcat(cmd->final_cmd," ");
        }

        if(tmp_quote[0] != 0){
            strcat(cmd->final_cmd, tmp_quote);
            strcat(cmd->final_cmd, token);
            strcat(cmd->final_cmd, tmp_quote);
        } else {
            strcat(cmd->final_cmd, token);
        }
        
    }

    cmd->argc = argc;
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