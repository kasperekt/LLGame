#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "server_queue.h"

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

int remove_queue(int queue_id) {
  return msgctl(queue_id, IPC_RMID, NULL);
}

int open_queue() {
  key_t queue_key = ftok(KEY_PATH, SERVER_MSGQ_ID);
  int queue_id = msgget(queue_key, IPC_CREAT | 0644);
  if (queue_id == -1) {
    perror("Opening queue error: ");
    exit(1);
  }
  return queue_id;
}

int send_queue_message(int queue_id, server_message_t *msg) {
  msg->mtype = 1;
  return msgsnd(queue_id, msg, sizeof(game_msg_t), 0);
}
