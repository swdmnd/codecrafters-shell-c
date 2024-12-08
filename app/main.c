#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"

int main() {
  while(1){
    printf("$ ");
    fflush(stdout);
    // Wait for user input
    char input[MAX_INPUT_BUFFER];
    fgets(input, MAX_INPUT_BUFFER, stdin);
    input[strcspn(input, "\r\n")] = 0;
    int err = parse_cmd(input, stdout);
    if(err == EXIT_0){
      return 0;
    } else if(err) {
      printf("%s: not found\n", input);
    }
  }
  

  return 0;
}
