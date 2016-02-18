#ifndef SEMAPHORE__OPERATIONS__H
#define SEMAPHORE__OPERATIONS__H

int create_sem();
int init_sem(int semid);
int sem_lock(int semid);
int sem_unlock(int semid);
void remove_sem(int semid);

#endif
