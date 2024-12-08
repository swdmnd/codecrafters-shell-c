#ifndef __CMD_PARSER_H__

#define MAX_INPUT_BUFFER 100
#define MAX_CMD_LENGTH 10
#define EXIT_0 0xFF
#define ERR_INVALID_CMD 0x01
#define ERR_CMD_LENGTH_LIMIT 0x02

extern int parse_cmd(char* cmd);

#endif
