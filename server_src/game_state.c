#include <stdlib.h>
#include <stdio.h>
#include "game_state.h"

game_state_t *players[2] = { NULL, NULL };

void init_players() {
  players[0] = NULL;
  players[1] = NULL;
}

void add_player(int player_id) {
  if (player_id < 1 || player_id > 2) {
    fprintf(stderr, "Trying to add new player failed (wrong id)\n");
    return;
  }

  int id = player_id - 1;
  if (players[id] != NULL) {
      fprintf(stderr, "Player already taken\n");
      return;
  }

  army_t *army = malloc(sizeof(struct army));
  army->light = 0;
  army->heavy = 0;
  army->cavalry = 0;
  army->workers = 0;

  players[id] = malloc(sizeof(struct game_state));
  players[id]->player_id = player_id;
  players[id]->resources = INITIAL_RESOURCES_COUNT;
  players[id]->army = army;
}

void destroy_players() {
  for (int i = 0; i < 2; ++i) {
    if (players[i] != NULL) {
      free(players[i]->army);
      free(players[i]);
    }
  }
}
