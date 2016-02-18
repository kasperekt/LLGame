#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "memory.h"
#include "game_state.h"

static int memory_id = -1;

void init_memory() {
  key_t key = ftok(MEMORY_KEY_PATH, MEMORY_KEY_ID);
  size_t size = game_state_size();
  int flags = IPC_CREAT | 0644;
  memory_id = shmget(key, size, flags);
  if (memory_id == -1) {
    perror("Error creating shared memory: ");
    exit(1);
  }
}

void remove_memory() {
  if (memory_id == -1) {
    return;
  }
  
  if (shmctl(memory_id, IPC_RMID, NULL) == -1) {
    perror("Error removing shared memory: ");
    exit(1);
  }
}
