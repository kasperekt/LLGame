#ifndef SEMAPHORE__OPERATIONS__H
#define SEMAPHORE__OPERATIONS__H

int initializeSemaphore(int semid);
int semLock(int semid);
int semUnlock(int semid);

#endif
