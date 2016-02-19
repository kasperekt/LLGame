#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../shared_src/game_protocol.h"
#include "game_state.h"
#include "memory.h"
#include "queue.h"

static const int RESOURCES_STEP = 50;

static game_state_t *players = NULL;
static game_state_t *mem_state = NULL;
static int players_online[2] = {0, 0};

int can_start() {
  return players_online[0] == 1 && players_online[1] == 1;
}

void init_players() {
  players_online[0] = 0;
  players_online[1] = 0;
  mem_state = NULL;
  players = malloc(game_state_size());
}

void add_player(int player_id) {
  if (player_id < 1 || player_id > 2) {
    fprintf(stderr, "Trying to add new player failed (wrong id)\n");
    return;
  }

  int id = player_id - 1;
  if (players_online[id] == 1) {
      fprintf(stderr, "Player already taken\n");
      return;
  }

  players_online[id] = 1;
  army_t army = {0, 0, 0, 0};
  players[id].wins = 0;
  players[id].resources = INITIAL_RESOURCES_COUNT;
  players[id].army = army;
}

void destroy_players() {
  if (players != NULL) {
    free(players);
  }
}

void start_game() {
  if (!can_start()) {
    fprintf(stderr, "Couldn't start the game (not all players are online)\n");
    return;
  }

  save_state();
  server_message_t msg = { 0, { GAME_START }};
  broadcast_message(&msg);
  printf("Game has started!\n");
  start_resources_production();
}

size_t game_state_size() {
  return 2 * sizeof(game_state_t);
}

void attach_state() {
  mem_state = get_memory_data(0);
  memcpy(players, mem_state, game_state_size());
}

void save_state() {
  if (mem_state == NULL) {
    mem_state = get_memory_data(0);
  }

  memcpy(mem_state, players, game_state_size());
  detach_memory_data(mem_state);
}

void broadcast_game_status() {
  for (int i = 0; i < 2; ++i) {
    const int rcp = i + 2;
    server_message_t msg = { rcp, {
      GAME_STATUS,
      { .status = { players[i].resources, 0, players[i].army }}
    }};
    send_message(&msg, rcp, 1);
  }
}

void increment_resources(int player_id) {
  attach_state();
  const int workers_count = players[player_id].army.workers;
  players[player_id].resources += RESOURCES_STEP + (workers_count * 5);
  save_state();
}

void remove_resources(int id, int resources) {
  attach_state();
  players[id].resources -= resources;
  save_state();
}

void add_unit(int id, army_type_t type) {
  attach_state();
  switch (type) {
    case LIGHT: {
      players[id].army.light += 1;
      break;
    }
    case HEAVY: {
      players[id].army.heavy += 1;
      break;
    }
    case CAVALRY: {
      players[id].army.cavalry += 1;
      break;
    }
    case WORKERS: {
      players[id].army.workers += 1;
      break;
    }
    default: break;
  }
  save_state();
}

void clear_army(int id) {
  players[id].army.light = 0;
  players[id].army.heavy = 0;
  players[id].army.cavalry = 0;
}

void casualties(army_t *army, float attack, float defence) {
  army->light -= floor(army->light * attack/defence);
  army->heavy -= floor(army->heavy * attack/defence);
  army->cavalry -= floor(army->cavalry * attack/defence);
}

void remove_units(int id, army_t a_army) {
  attach_state();
  players[id].army.light -= a_army.light;
  players[id].army.heavy -= a_army.heavy;
  players[id].army.cavalry -= a_army.cavalry;
  save_state();
}

void add_units(int id, army_t army) {
  players[id].army.light += army.light;
  players[id].army.heavy += army.heavy;
  players[id].army.cavalry += army.cavalry;
}

void finish_game(int winner) {
  server_message_t msg = { 2, {
    GAME_RESULT,
    { .game_result = { winner }}
  }};
  broadcast_message(&msg);
  printf("Game has ended\n");
}

int attack(int a_id, int d_id, army_t a_army) {
  int winner = 0;
  attach_state();

  float defence = army_defence(players[d_id].army);
  float attack = army_attack(players[a_id].army);
  float def_attack = army_attack(players[d_id].army);
  float att_defence = army_defence(players[a_id].army);

  if ((attack - defence) > 0) {
    clear_army(d_id);
    winner = a_id + 1;
    players[d_id].wins += 1;
  } else {
    casualties(&players[d_id].army, attack, defence);
    winner = d_id + 1;
    players[d_id].wins += 1;
  }

  if ((def_attack - att_defence) <= 0) {
    casualties(&a_army, def_attack, att_defence);
    add_units(a_id, a_army);
  }

  if (players[a_id].wins == 5 || players[d_id].wins == 5) {
    finish_game(winner);
  }

  save_state();
  return winner;
}
