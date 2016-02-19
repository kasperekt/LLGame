#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../shared_src/game_protocol.h"
#include "game_state.h"
#include "memory.h"
#include "queue.h"

static const int RESOURCES_STEP = 50;

static game_state_t *players[2] = { NULL, NULL };
static game_state_t **mem_state = NULL;

int can_start() {
  return !(players[0] == NULL || players[1] == NULL);
}

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
  players[id]->wins = 0;
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
  return 2 * (6 * sizeof(int));
}

void attach_state() {
  mem_state = get_memory_data(0);
  players[0] = mem_state[0];
  players[1] = mem_state[1];
  // memcpy(&players, &mem_state, game_state_size());
}

void save_state() {
  if (mem_state == NULL) {
    mem_state = get_memory_data(0);
  }

  mem_state[0] = players[0];
  mem_state[1] = players[1];
  // memcpy(&mem_state, &players, game_state_size());
  detach_memory_data(mem_state);
}

void broadcast_game_status() {
  for (int i = 0; i < 2; ++i) {
    const int rcp = i + 2;
    server_message_t msg = { rcp, {
      GAME_STATUS,
      { .status = { players[i]->resources, 0, *(players[i]->army) }}
    }};
    send_message(&msg, rcp, 1);
  }
}

void increment_resources(int player_id) {
  attach_state();
  const int workers_count = players[player_id]->army->workers;
  players[player_id]->resources += RESOURCES_STEP + (workers_count * 5);
  save_state();
}

void add_unit(int id, army_type_t type) {
  attach_state();
  switch (type) {
    case LIGHT: {
      players[id]->army->light += 1;
      break;
    }
    case HEAVY: {
      players[id]->army->heavy += 1;
      break;
    }
    case CAVALRY: {
      players[id]->army->cavalry += 1;
      break;
    }
    case WORKERS: {
      players[id]->army->workers += 1;
      break;
    }
    default: break;
  }
  printf("R: [%d], Army: [%d], [%d], [%d], [%d]\n",
    players[id]->resources,
    players[id]->army->light,
    players[id]->army->heavy,
    players[id]->army->cavalry,
    players[id]->army->workers
  );
  save_state();
}
