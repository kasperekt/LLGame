#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "../shared_src/game_protocol.h"
#include "queue.h"
#include "game_state.h"
#include "memory.h"

void init() {
  init_queue();
  init_memory();
  init_players();
}

void cleanup() {
  remove_queue();
  remove_memory();
  destroy_players();
}

void sig_int() {
  cleanup();
  exit(SIGINT);
}

int main() {
  signal(SIGINT, sig_int);
  init();

  server_message_t message;
  while (get_queue_message(&message) > 0) {
    switch (message.mdata.action_type) {
      case CONNECT: {
        int client_id = message.mdata.data.client_id;
        add_player(client_id);
        server_message_t cmsg = { client_id, { CONNECT, { .client_id = client_id }}};
        send_queue_message(&cmsg, client_id + 1);
        printf("Client [%d] has connected\n", client_id);
        if (can_start()) {
          start_game();
        }
        break;
      }
      case UNIT_TRAINING: {
        int client_id = message.mdata.data.training.player_id - 1;
        int type = message.mdata.data.training.type;
        int count = message.mdata.data.training.count;
        printf("Got training request of unit <%s> (%d times) from client %d\n",
          unit_string(type),
          count,
          client_id
        );
        start_training(client_id, type, count);
        break;
      }
      case ATTACK: {
        int attacker_id = message.mdata.data.attack.attacker_id - 1;
        int defender_id = attacker_id == 0 ? 1 : 0;
        army_t attacker_army = message.mdata.data.attack.army;
        printf("Got message: Attacker [%d], Defender [%d] with army [%d, %d, %d]\n",
          attacker_id,
          defender_id,
          attacker_army.light,
          attacker_army.heavy,
          attacker_army.cavalry
        );
        start_attack(attacker_id, defender_id, attacker_army);
        break;
      }
      default:
        break;
    }
  }

  cleanup();
  return 0;
}
