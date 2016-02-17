#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "server_queue.h"

void listen_to_server(int queue_id) {
  server_message_t msg;
  while (get_queue_message(queue_id, &msg) > 0) {
    switch (msg.mdata.action_type) {
      case CONNECT: {
        int client_id = msg.mdata.data.client_id;
        if (client_id != cid) {
          fprintf(stderr, "Couldn't connect to server\n");
          exit(1);
        }

        printf("Successfully connected to server!\n");
        break;
      }
      default:
        break;
    }
  }
}
