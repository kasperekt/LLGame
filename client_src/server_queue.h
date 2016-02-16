#ifndef SERVER__QUEUE__H
#define SERVER__QUEUE__H

typedef struct msgbuf {
  long mtype;
  char message[10];
} server_message_t;

int open_queue();
int remove_queue(int queue_id);

#endif
