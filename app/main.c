#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"

int main() {
  while(1){
    printf("$ ");
    fflush(stdout);
    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    input[strcspn(input, "\r\n")] = 0;
    int err = parse_cmd(input);
    if(err == EXIT_0){
      return 0;
    } else if(err) {
      printf("%s: not found\n", input);
    }
  }
  

  return 0;
}
