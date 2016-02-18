#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void listen_to_server() {
  server_message_t msg;
  while (get_queue_message(&msg) > 0) {
    switch (msg.mdata.action_type) {
      case CONNECT: {
        int client_id = msg.mdata.data.client_id;
        if (client_id != cid) {
          fprintf(stderr, "Couldn't connect to server\n");
          exit(1);
        }
        printf("Successfully connected\n");
        break;
      }
      case GAME_START: {
        printf("Game has started!\n");
        break;
      }
      default:
        break;
    }
  }
}
