#ifndef __SHELL_H__

#define MAX_INPUT_BUFFER 100
#define MAX_CMD_LENGTH 100
#define EXIT_0 0xFF
#define ERR_INVALID_CMD 0x01
#define ERR_TOKEN_LENGTH_LIMIT 0x02
#define ERR_FILE_NOT_FOUND 0x03

extern char* ENV_PATH;

extern int parse_cmd(char* cmd, FILE* out_stream);

#endif
