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
  // int queue_id = open_queue();

  // while (1) {
  //   char *command = get_command();
  //   strcpy(msg.message, command);
  //   send_queue_message(queue_id, &msg);
  // }

  return 0;
}
