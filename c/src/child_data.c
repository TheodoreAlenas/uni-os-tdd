#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>

#include "constants.h"
#include "dev_mode.h"
#include "child_data.h"


ChildData *child_data_malloc(int num_of_workers) {
  int i;

  ChildData * c = malloc(num_of_workers * sizeof(ChildData));
  for (i = 0; i < num_of_workers; i++)
    c[i].sem_name = NULL;

  return c;
}

int child_data_create(ChildData *c, char *sem_name) {
  c->is_working = false;
  c->current_file_segment = 0;

  c->sem_name = sem_name;
  c->semaphore = sem_open(sem_name, O_CREAT | O_WRONLY, 0666, 0);
  if (c->semaphore == NULL) {
    perror("chld_data_create sem_open");
    child_data_free_one(c);
    return -1;
  }

  return 0;
}

void child_data_free_one(ChildData *c) {

  if (c == NULL)
    return;

  if (c->semaphore) {
    sem_unlink(c->sem_name);
    sem_close(c->semaphore);
  }
}


void child_data_free_all(ChildData *c, unsigned num_of_children) {
  int i;

  for (i = 0; i < num_of_children; i++)
    child_data_free_one(c + i);

  free(c);
}
