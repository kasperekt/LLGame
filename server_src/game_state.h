#ifndef GAME__STATE__H
#define GAME__STATE__H
#include "../shared_src/game_protocol.h"

#define INITIAL_RESOURCES_COUNT 300

typedef struct game_state {
  short player_id;
  unsigned int resources;
  army_t *army;
} game_state_t;

extern game_state_t *players[2];

void init_players();
void add_player(int player_id);
void destroy_players();

#endif
