#ifndef CLIENT__QUEUE__H
#define CLIENT__QUEUE__H
#include "../shared_src/game_protocol.h"

int open_queue();
int remove_queue(int queue_id);
int get_queue_id();
ssize_t get_queue_message(int queue_id, server_message_t *msg);
int send_queue_message(int queue_id, server_message_t *msg, int rcp);
void broadcast_message(int queue_id, server_message_t *msg);

#endif
