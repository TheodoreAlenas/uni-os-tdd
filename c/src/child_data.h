#ifndef CHILD_DATA_H
#define CHILD_DATA_H

#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>


typedef struct child_data {
  pid_t pid;
  sem_t *semaphore;
  bool is_working;
  int current_file_segment;
  unsigned num_of_children;
} ChildData;

ChildData *child_data_create_all(int num_of_children);
ChildData *child_data_create_one(unsigned index, pid_t pid);
void child_data_free(ChildData *c);

#endif
