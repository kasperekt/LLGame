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

int get_client_id(char **argv) {
  if (strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0) {
    return -1;
  }

  char id_char = argv[1][0];
  return (int)(id_char - '0');
}
