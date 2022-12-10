#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"

typedef struct {
  sem_t *childs_semaphore;
  int segment;
} Req;

int testable_wait(const Parent *r);
int testable_post(const Parent *r, unsigned child);
int testable_sprintf(void *shm, char *str1, char *str2);
Req testable_parse_req(const void *shm);
char *testable_read_file_segment(const Parent *parent, unsigned long segment);

int loops(Parent *r, int children, int per_child);
int one_cycle(Parent *r);

#endif
