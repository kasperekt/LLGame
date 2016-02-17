#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "io_tools.h"
#include "server_queue.h"

int get_client_id(char **argv) {
  if (strcmp(argv[1], "1") != 0 && strcmp(argv[1], "2") != 0) {
    return -1;
  }

  char id_char = argv[1][0];
  return (int)(id_char - '0');
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "You need to pass client ID (1 or 2)\n");
    exit(1);
  }

  int client_id = get_client_id(argv);
  if (client_id == -1) {
    fprintf(stderr, "You have passed wrong ID (Should be 1 or 2)\n");
    exit(1);
  }

  int queue_id = connect(client_id);
  server_message_t msg;

  while (1) {
    printf("Name: ");
    char *name = get_command();
    printf("Surname: ");
    char *surname = get_command();
    msg.mdata.action_type = EXAMPLE;
    strcpy(msg.mdata.data.test.name, name);
    strcpy(msg.mdata.data.test.surname, surname);
    printf("Sending! (client)\n");

    if (send_queue_message(queue_id, &msg) == -1) {
      perror("Client sending message: ");
      exit(1);
    }
  }

  return 0;
}
