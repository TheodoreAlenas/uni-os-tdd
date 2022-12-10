#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"

int testable_wait(sem_t *sem);
int testable_post(sem_t *sem);
int testable_sprintf(void *shm, char *str1, char *str2);
char *testable_shm_read(void *shm);
char *parent_read_file_segment(const Parent *parent, unsigned long segment);

#endif
