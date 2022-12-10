#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"

typedef struct {
  sem_t *childs_semaphore;
  int segment;
} Req;

int testable_wait(sem_t *sem);
int testable_post(sem_t *sem);
int testable_sprintf(void *shm, char *str1, char *str2);
Req testable_parse_req(void *shm);
char *parent_read_file_segment(const Parent *parent, unsigned long segment);

#endif
