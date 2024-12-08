#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

char* ENV_PATH = NULL;

int main() {
  ENV_PATH = getenv("PATH");
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
