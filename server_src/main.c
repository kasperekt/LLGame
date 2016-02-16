#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "client_queue.h"
#include "../shared_src/game_protocol.h"

int main() {
  int client_queue_id = open_queue();
  server_message_t message;

  while (get_queue_message(client_queue_id, &message) > 0) {
    if (message.mdata.action_type == EXAMPLE) {
      struct example data = message.mdata.data.test;
      printf("You are %s %s\n", data.name, data.surname);
    }
  }

  if (remove_queue(client_queue_id) == -1) {
    perror("Removing queue: ");
    exit(1);
  }
  return 0;
}
