#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

#define SEM_KEY_PATH "/tmp"
static int sem_key_id = 55;

int create_sem() {
  key_t key = ftok(SEM_KEY_PATH, sem_key_id++);
  const int n_sem = 1;
  const int flag = IPC_CREAT | 0644;

  int sem_id = semget(key, n_sem, flag);
  if (sem_id == -1) {
    perror("Error creating semaphore: ");
    exit(1);
  }

  return sem_id;
}

int init_sem(int semid) {
  union semun argument;
  unsigned short values[1];
  values[0] = 1;
  argument.array = values;
  return semctl(semid, 0, SETALL, argument);
}

int sem_unlock(int semid) {
  struct sembuf operations[1];
  operations[0].sem_num = 0;
  operations[0].sem_op = 1;
  operations[0].sem_flg = 0;
  return semop(semid, operations, 1);
}

int sem_lock(int semid) {
  struct sembuf operations[1];
  operations[0].sem_num = 0;
  operations[0].sem_op = -1;
  operations[0].sem_flg = 0;
  return semop(semid, operations, 1);
}

void remove_sem(int semid) {
  if (semctl(semid, 0, IPC_RMID, 0) == -1) {
    perror("Error removing semaphore: ");
    exit(1);
  }
}
