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
  server_message_t msg = {1, "Hey!"};
  int queue_id = open_queue();

  while (1) {
    char *command = get_command();
    strcpy(msg.message, command);
    msgsnd(queue_id, &msg, 10, 0);
  }

  return 0;
}
