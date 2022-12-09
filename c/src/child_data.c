#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>

#include "child_data.h"
#include "get_names.h"


ChildData *child_data_create_all(int num_of_workers) {
  /* TODO malloc fail case */
  ChildData * c = malloc(num_of_workers * sizeof(ChildData));
  for (int i = 0; i < num_of_workers; i++) {
    c[i].is_working = false;
    c[i].semaphore = sem_open(get_semaphore_name(i), O_CREAT | O_WRONLY, 0666, 0);
    if (c[i].semaphore == NULL) {
      perror("chld_data_create sem_open");
      child_data_free(c);
      return NULL;
    }
    c[i].current_file_segment = 0;
  }
  return c;
}

void child_data_free(ChildData *c) {
  int i;
  for (i = 0; i < c->num_of_children; i++) {
    if (c[i].semaphore) {
      sem_unlink(get_semaphore_name(i));
      sem_close(c[i].semaphore);
    }
  }
  free(c);  /* TODO clear memory */
}


