#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "semaphore_operations.h"

#define MEM_SIZE 64
#define SEM_SIZE 1

void cleanup(int memid, int semid) {
  struct shmid_ds shm_desc;
  shmctl(memid, IPC_RMID, &shm_desc);
  semctl(semid, SEM_SIZE, IPC_RMID);
}

int main() {
  const char *path = "/tmp";
  key_t memKey = ftok(path, 1);
  key_t semKey = ftok(path, 2);
  int semid, memid;

  if ((memid = shmget(memKey, MEM_SIZE, IPC_CREAT | 0644)) == -1) {
      perror("Shared memory creation error");
      exit(1);
  }

  if ((semid = semget(semKey, SEM_SIZE, IPC_CREAT | 0644)) == -1) {
    perror("Semaphore creation fail");
    cleanup(memid, semid);
    exit(1);
  }
  initializeSemaphore(semid);

  sleep(2);
  cleanup(memid, semid);
  return 0;
}
