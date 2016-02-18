#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "memory.h"
#include "queue.h"
#include "semaphore.h"
#include "game_state.h"

static int memory_id = -1;
static int memory_sem = -1;
static pid_t resources_pid = -1;

void init_memory() {
  key_t key = ftok(MEMORY_KEY_PATH, MEMORY_KEY_ID);
  size_t size = game_state_size();
  int flags = IPC_CREAT | 0644;
  memory_id = shmget(key, size, flags);
  if (memory_id == -1) {
    perror("Error creating shared memory: ");
    exit(1);
  }
  memory_sem = create_sem();
}

void remove_memory() {
  remove_sem(memory_sem);
  if (memory_id == -1) {
    return;
  }

  if (shmctl(memory_id, IPC_RMID, NULL) == -1) {
    perror("Error removing shared memory: ");
    exit(1);
  }
}

void start_resources_production() {
  pid_t pid = fork();
  switch (pid) {
    case -1: {
      perror("Error forking (resources): ");
      exit(1);
    }
    case 0: {
      while (1) {
        sem_lock(memory_sem);
        increment_resources(0);
        increment_resources(1);
        broadcast_game_status();
        sem_unlock(memory_sem);
        sleep(1);
      }
      exit(0);
    }
    default: {
      resources_pid = pid;
      break;
    }
  }
}

game_state_t **get_memory_data(char *shmaddr) {
  const int shmat_flag = 0;
  return shmat(memory_id, shmaddr, shmat_flag);
}

void detach_memory_data(game_state_t **data) {
  shmdt(data);
}
