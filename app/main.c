#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"

int main() {
  printf("$ ");
  fflush(stdout);

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);
  input[strcspn(input, "\r\n")] = 0;
  int err = parse_cmd(input);
  if(err){
    printf("%s: not found\n", input);
  }

  return err;
}
