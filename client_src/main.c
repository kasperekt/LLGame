#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "io_tools.h"

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

typedef struct msgbuf {
  long mtype;
  char message[10];
} server_message_t;

int open_server_queue() {
  key_t server_queue_path = ftok(KEY_PATH, SERVER_MSGQ_ID);
  int queue_id = msgget(server_queue_path, 0);
  if (queue_id == -1) {
    perror("Opening queue error: ");
    exit(1);
  }
  return queue_id;
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

int main() {
  server_message_t msg = {1, "Hey!"};
  int queue_id = open_server_queue();

  while (1) {
    char *command = get_command();
    strcpy(msg.message, command);
    msgsnd(queue_id, &msg, 10, 0);
  }

  return 0;
}
