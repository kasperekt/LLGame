#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "server_queue.h"

#define KEY_PATH "/tmp"
#define SERVER_MSGQ_ID 19

int cid = -1;

int remove_queue(int queue_id) {
  return msgctl(queue_id, IPC_RMID, NULL);
}

int open_queue() {
  key_t queue_key = ftok(KEY_PATH, SERVER_MSGQ_ID);
  int queue_id = msgget(queue_key, 0);
  if (queue_id == -1) {
    perror("Opening queue error (check if server is running): ");
    exit(1);
  }
  return queue_id;
}

int send_queue_message(int queue_id, server_message_t *msg) {
  msg->mtype = 1;
  return msgsnd(queue_id, msg, sizeof(game_message_t), 0);
}

ssize_t get_queue_message(int queue_id, server_message_t *msg) {
  return msgrcv(queue_id, msg, sizeof(game_message_t), cid + 1, 0);
}

int connect(int client_id) {
  int queue_id = open_queue();
  cid = client_id;
  game_message_t game_msg;
  game_msg.action_type = CONNECT;
  game_msg.data.client_id = client_id;
  server_message_t msg;
  msg.mdata = game_msg;
  if (send_queue_message(queue_id, &msg) == -1) {
    perror("Couldn't connect to server: ");
    exit(1);
  }
  return queue_id;
}
