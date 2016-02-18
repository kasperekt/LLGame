#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "queue.h"

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

static int queue_id = -1;

void remove_queue() {
  if (msgctl(queue_id, IPC_RMID, 0) == -1) {
    perror("Error removing queue: ");
    exit(1);
  }
}

void init_queue() {
  key_t queue_key = ftok(KEY_PATH, SERVER_MSGQ_ID);
  queue_id = msgget(queue_key, IPC_CREAT | 0644);
  if (queue_id == -1) {
    perror("Opening queue error: ");
    exit(1);
  }
}

int get_queue_id() {
  return queue_id;
}

ssize_t get_queue_message(server_message_t *msg) {
  return msgrcv(queue_id, msg, sizeof(game_message_t), 1, 0);
}

int send_queue_message(server_message_t *msg, int rcp) {
  msg->mtype = rcp;
  return msgsnd(queue_id, msg, sizeof(game_message_t), 0);
}

void broadcast_message(server_message_t *msg) {
  if (send_queue_message(msg, 2) == -1) {
    perror("Error broadcasting message (PLAYER ONE): ");
    exit(1);
  }

  if (send_queue_message(msg, 3) == -1) {
    perror("Error broadcasting message (PLAYER TWO): ");
    exit(1);
  }
}
