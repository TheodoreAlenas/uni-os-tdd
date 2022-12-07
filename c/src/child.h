#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct child_arguments {
  char *sem_name_i_want;
  char *sem_name_thank_you;
  char *shmem_name_i_want;
  char *shmem_name_thank_you;
} ChildArgs;

typedef struct child_responce {
  int application_time_in_ns;
  int responce_time_in_ns;
  int file_segment;
  int line_in_segment;
  char *line_contents;
} ChildRes;

typedef struct child {
  sem_t sem_i_want;
  sem_t sem_thank_you;
  void *shmem_i_want;
  void *shmem_thank_you;
} Child;

Child *child_create(ChildArgs args);
void child_free(Child *child);

ChildRes *child_res_create();
void child_res_free(ChildRes *res);
int child_res_to_ostream(ChildRes *res, FILE *file);
int child_res_to_file(ChildRes *res, char *filename);

#endif
