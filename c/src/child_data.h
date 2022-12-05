#ifndef CHILD_DATA_H
#define CHILD_DATA_H

#include <stdbool.h>
#include <stdlib.h>


typedef struct child_data {
  int semaphore;
  void *shmem;
  bool is_working;
  int current_file_segment;
} ChildData;

ChildData *child_data_create(int num_of_children);
void child_data_free(ChildData *c);

#endif
