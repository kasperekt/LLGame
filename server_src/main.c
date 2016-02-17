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
    switch (message.mdata.action_type) {
      case CONNECT: {
        int client_id = message.mdata.data.client_id;
        printf("Client [%d] is trying to connect\n", client_id);
        break;
      }
      case UNIT_TRAINING: {
        int type = message.mdata.data.training.type;
        int count = message.mdata.data.training.count;
        printf("Got training request of unit %d (%d times)\n", type, count);
        break;
      }
      default:
        break;
    }
  }

  if (remove_queue(client_queue_id) == -1) {
    perror("Removing queue: ");
    exit(1);
  }
  return 0;
}
