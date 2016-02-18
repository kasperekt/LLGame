#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "queue.h"

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

int cid = -1;
static int queue_id = -1;

void remove_queue() {
  if (queue_id == -1) {
    return;
  }

  if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
    perror("Error removing message queue: ");
    exit(1);
  }
}

void open_queue() {
  key_t queue_key = ftok(KEY_PATH, SERVER_MSGQ_ID);
  queue_id = msgget(queue_key, 0);
  if (queue_id == -1) {
    perror("Opening queue error (check if server is running): ");
    exit(1);
  }
}

int send_queue_message(server_message_t *msg) {
  msg->mtype = 1;
  return msgsnd(queue_id, msg, sizeof(game_message_t), 0);
}

ssize_t get_queue_message(server_message_t *msg) {
  return msgrcv(queue_id, msg, sizeof(game_message_t), cid + 1, 0);
}

void connect(int client_id) {
  open_queue();
  cid = client_id;
  server_message_t msg = { 1, { CONNECT, { .client_id = client_id }}};
  if (send_queue_message(&msg) == -1) {
    perror("Couldn't connect to server: ");
    exit(1);
  }
}
