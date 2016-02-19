#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "io_tools.h"

void clear_terminal() {
  printf("\033[2J\033[1;1H");
  rewind(stdout);
}

char *remove_last_char(char *str) {
  str[strlen(str) - 1] = '\0';
  return str;
}

command_t get_command() {
  char *data = 0;
  size_t sth = 0;
  getline(&data, &sth, stdin);
  char *command = remove_last_char(data);
  if (strcmp(command, "train") == 0) {
    return TRAIN_COMMAND;
  } else if (strcmp(command, "attack") == 0) {
    return ATTACK_COMMAND;
  }

  return WRONG_COMMAND;
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
      printf("Commands: 'attack', 'train'\n");
      printf("-> ");
      switch(get_command()) {
          case TRAIN_COMMAND: {
            printf("Training\n");
            sleep(1);
            break;
          }
          case ATTACK_COMMAND: {
            printf("Attack!\n");
            sleep(1);
            break;
          }
          default: break;
      }
      kill(child_pid, SIGCONT);
    }
  }
}
