#ifndef GAME__STATE__H
#define GAME__STATE__H
#include "../shared_src/game_protocol.h"
#include "../shared_src/game_world.h"

#define INITIAL_RESOURCES_COUNT 300

typedef struct game_state {
  int resources;
  int wins;
  army_t army;
} game_state_t;

void init_players();
void add_player(int player_id);
void destroy_players();
void start_game();
void attach_state();
void save_state();
int can_start();
void increment_resources(int player_id);
void remove_resources(int id, int resources);
void add_unit(int id, army_type_t type);
void remove_units(int id, army_t a_army);
int attack(int a_id, int d_id, army_t a_army);
void broadcast_game_status();
size_t game_state_size();
void finish_game(int winner);

#endif
