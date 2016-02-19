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
#include "../shared_src/game_world.h"

static int memory_id = -1;
static int memory_sem = -1;
static pid_t resources_pid = -1;

void init_memory() {
  key_t key = ftok(MEMORY_KEY_PATH, MEMORY_KEY_ID);
  size_t size = game_state_size();
  printf("SIZE IS %d\n", (int)size);
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

void start_training(int id, army_type_t type, int count) {
  pid_t pid = fork();
  switch (pid) {
    case -1: {
      perror("Error forking (training): ");
      exit(1);
    }
    case 0: {
      int resources = unit_cost(type) * count;
      sem_lock(memory_sem);
      remove_resources(id, resources);
      sem_unlock(memory_sem);

      while (count--) {
        int unit_counter = unit_training_time(type);
        while (unit_counter--) sleep(1);

        sem_lock(memory_sem);
        add_unit(id, type);
        sem_unlock(memory_sem);
      }
      exit(0);
    }
    default: {
      break;
    }
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
        if (sem_lock(memory_sem) == -1) {
            perror("Semaphore locking error: ");
            exit(1);
        }

        increment_resources(0);
        increment_resources(1);
        broadcast_game_status();

        if (sem_unlock(memory_sem) == -1) {
            perror("Semaphore unlocking error: ");
            exit(1);
        }

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

void start_attack(int a_id, int d_id, army_t a_army) {
  pid_t pid = fork();
  switch (pid) {
    case -1: {
      perror("Error forking (training): ");
      exit(1);
    }
    case 0: {
      sem_lock(memory_sem);
      remove_units(a_id, a_army);
      sem_unlock(memory_sem);

      sleep(5);

      sem_lock(memory_sem);
      int winner = attack(a_id, d_id, a_army);
      sem_unlock(memory_sem);

      server_message_t msg = { 2, { ATTACK_RESULT, { .attack_result = { winner }}}};
      broadcast_message(&msg);
      exit(0);
    }
    default: {
      break;
    }
  }
}

game_state_t *get_memory_data(char *shmaddr) {
  const int shmat_flag = 0;
  return shmat(memory_id, shmaddr, shmat_flag);
}

void detach_memory_data(game_state_t *data) {
  if (shmdt(data) == -1) {
    perror("Error detaching memory: ");
    exit(1);
  };
}
