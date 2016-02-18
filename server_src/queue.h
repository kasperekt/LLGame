#ifndef CLIENT__QUEUE__H
#define CLIENT__QUEUE__H
#include "../shared_src/game_protocol.h"

void init_queue();
void remove_queue();
int get_queue_id();
ssize_t get_queue_message(server_message_t *msg);
int send_queue_message(server_message_t *msg, int rcp);
void broadcast_message(server_message_t *msg);

#endif
