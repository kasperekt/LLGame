#ifndef SERVER__QUEUE__H
#define SERVER__QUEUE__H

typedef struct msgbuf {
  long mtype;
  char message[10];
} server_message_t;

int open_queue();
int remove_queue(int queue_id);
int send_queue_message(int queue_id, server_message_t *msg);

#endif
