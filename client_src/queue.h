#ifndef SERVER__QUEUE__H
#define SERVER__QUEUE__H
#include "../shared_src/game_protocol.h"

extern int cid;

void open_queue();
void connect(int client_id);
void remove_queue();
int send_queue_message(server_message_t *msg);
ssize_t get_queue_message(server_message_t *msg);

#endif
