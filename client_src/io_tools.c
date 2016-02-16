#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *remove_last_char(char *str) {
  str[strlen(str) - 1] = '\0';
  return str;
}

char *get_command() {
  char *data = 0;
  size_t sth = 0;
  getline(&data, &sth, stdin);
  return remove_last_char(data);
}
