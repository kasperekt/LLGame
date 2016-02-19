#ifndef MEMORY__H
#define MEMORY__H
#include "game_state.h"

#define MEMORY_KEY_PATH "/tmp"
#define MEMORY_KEY_ID 20

void init_memory();
void remove_memory();
void start_resources_production();
void start_training(int id, army_type_t type, int count);
game_state_t *get_memory_data(char *shmaddr);
void detach_memory_data(game_state_t *data);

#endif
