#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "client_queue.h"

int main() {
  int client_queue_id = open_queue();
  server_message_t message;
  printf("Queue START!\n");

  while (get_queue_message(client_queue_id, &message) > 0) {
    printf("Message from client!");
  }

  printf("Queue OVER!\n");
  if (remove_queue(client_queue_id) == -1) {
    perror("Removing queue: ");
    exit(1);
  }
  return 0;
}
