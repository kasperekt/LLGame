#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void clear_terminal() {
  printf("\033[2J\033[1;1H");
  rewind(stdout);
}

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

void read_command(pid_t child_pid) {
  char c;
  while ((c = getchar())) {
    if (c == '\n') {
      kill(child_pid, SIGSTOP);
      printf("-> ");
      char *new_command = get_command();
      printf("Command[%s]\n", new_command);
      kill(child_pid, SIGCONT);
    }
  }
}
