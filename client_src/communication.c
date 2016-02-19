#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../shared_src/game_protocol.h"
#include "queue.h"
#include "io_tools.h"

void listen_to_server() {
  server_message_t msg;
  while (get_queue_message(&msg) > 0) {
    switch (msg.mdata.action_type) {
      case CONNECT: {
        int client_id = msg.mdata.data.client_id;
        if (client_id != cid) {
          fprintf(stderr, "Couldn't connect to server\n");
          exit(1);
        }
        printf("Successfully connected\n");
        break;
      }
      case GAME_START: {
        printf("Game has started!\n");
        break;
      }
      case GAME_STATUS: {
        game_status_t status = msg.mdata.data.status;
        clear_terminal();
        printf("SCORE: %d | Resources: %d | Army: L[%d] H[%d] C[%d] W[%d]\n",
          status.wins,
          status.resources,
          status.army.light,
          status.army.heavy,
          status.army.cavalry,
          status.army.workers
        );
        break;
      }
      case ATTACK_RESULT: {
        int winner_id = msg.mdata.data.attack_result.winner_id;
        printf("Player %d has won the battle\n", winner_id);
        sleep(1);
        break;
      }
      case GAME_RESULT: {
        int winner_id = msg.mdata.data.game_result.winner_id;

        if (winner_id == cid) {
          printf("\n----- WINNER!!! -----\n");
        } else {
          printf("DEFEATED!\n");
        }

        sleep(5);
        exit(0);
        break;
      }
      default:
        break;
    }
  }
}
