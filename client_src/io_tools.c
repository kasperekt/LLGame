#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io_tools.h"
#include "../shared_src/game_protocol.h"
#include "communication.h"
#include "queue.h"
#include "../shared_src/game_world.h"

void clear_terminal() {
  printf("\033[2J\033[1;1H");
  rewind(stdout);
}

int get_status(game_status_t *status) {
  server_message_t msg;
  get_queue_message(&msg);
  if (msg.mdata.action_type != GAME_STATUS) {
    fprintf(stderr, "Error getting game status. Try again\n");
    return -1;
  }

  *status = msg.mdata.data.status;
  return 0;
}

char *remove_last_char(char *str) {
  str[strlen(str) - 1] = '\0';
  return str;
}

char *get_command_string() {
  char *data = 0;
  size_t sth = 0;
  printf("-> ");
  getline(&data, &sth, stdin);
  return remove_last_char(data);
}

command_t get_command() {
  printf("Commands: 'attack', 'train'\n");
  char *command = get_command_string();
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
      switch(get_command()) {
          case TRAIN_COMMAND: {
            clear_terminal();
            game_status_t status;
            if (get_status(&status) == -1) {
              break;
            }

            int resources = status.resources;
            printf("-- TRAINING --\n");
            printf("You have [%d] resources\n", resources);
            printf("What units do you want to train: 'light', 'heavy', 'cavalry', 'worker'\n");
            army_type_t army_type = get_army_type(get_command_string());
            while (army_type == 0) {
              printf("Wrong type passed. Try again\n");
              army_type = get_army_type(get_command_string());
            }

            int cost = unit_cost(army_type);
            int max = resources / cost;
            if (max == 0) {
              printf("You don't have enough resouces\n");
              sleep(1);
              break;
            }

            printf("You can train max [%d]\nHow many do you want?\n", max);
            int count = atoi(get_command_string());
            while (count > max) {
              printf("You don't have enough resources\n");
              count = atoi(get_command_string());
            }

            server_message_t msg = { 1, {
              UNIT_TRAINING,
              { .training = {
                  cid,
                  army_type,
                  count
              }}
            }};
            send_queue_message(&msg);
            printf("Started training for [%d] units\n", count);
            sleep(1);
            break;
          }
          case ATTACK_COMMAND: {
            printf("Attack!\n");
            sleep(1);
            break;
          }
          default: {
            fprintf(stderr, "default\n");
            break;
          }
      }
      kill(child_pid, SIGCONT);
    }
  }
}
