#ifndef CHILD_DATA_H
#define CHILD_DATA_H

#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>


typedef struct child_data {
  pid_t pid;
  char *sem_name;
  sem_t *semaphore;
  bool is_working;
  int current_file_segment;
} ChildData;

ChildData *child_data_malloc(int num_of_children);
int child_data_create(ChildData *c, char *sem_name);
void child_data_free_all(ChildData *c, unsigned num_of_children);
void child_data_free_one(ChildData *c);

#endif
