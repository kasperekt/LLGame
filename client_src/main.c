#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "io_tools.h"
#include "communication.h"
#include "queue.h"

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

  connect(client_id);
  pid_t pid = fork();
  switch (pid) {
    case -1: {
      perror("Error forking: ");
      exit(1);
      break;
    }
    case 0: {
      listen_to_server();
      break;
    }
    default: {
      read_command(pid);
    }
  }
  return 0;
}
