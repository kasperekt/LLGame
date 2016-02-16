#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

int remove_queue(int queue_id) {
  return msgctl(queue_id, IPC_RMID, NULL);
}

int open_queue() {
  key_t server_queue_path = ftok(KEY_PATH, SERVER_MSGQ_ID);
  int queue_id = msgget(server_queue_path, IPC_CREAT | 0644);
  if (queue_id == -1) {
    perror("Opening queue error: ");
    exit(1);
  }
  return queue_id;
}
