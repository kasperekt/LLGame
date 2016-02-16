#ifndef CLIENT__QUEUE__H
#define CLIENT__QUEUE__H

typedef struct msgbuf {
  long mtype;
  char message[10];
} client_message_t;

int open_queue();
int remove_queue(int queue_id);

#endif
