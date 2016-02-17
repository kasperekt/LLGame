#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "io_tools.h"
#include "server_queue.h"

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

  return 0;
}
