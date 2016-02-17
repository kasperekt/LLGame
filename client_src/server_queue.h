#ifndef SERVER__QUEUE__H
#define SERVER__QUEUE__H
#include "../shared_src/game_protocol.h"

int open_queue();
int connect(int client_id);
int remove_queue(int queue_id);
int send_queue_message(int queue_id, server_message_t *msg);

#endif
