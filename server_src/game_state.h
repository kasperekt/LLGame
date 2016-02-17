#ifndef GAME__STATE__H
#define GAME__STATE__H
#include "../shared_src/game_protocol.h"

typedef struct game_state {
  unsigned int resources;
  army_t army;
} game_state_t;

typedef struct players {
  short player_one;
  short player_two;
} players_t;

#endif
