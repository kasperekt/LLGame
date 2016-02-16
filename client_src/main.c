#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "io_tools.h"
#include "server_queue.h"

int main() {
  int queue_id = open_queue();
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
