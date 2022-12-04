#include "child_data.h"


ChildData *child_data_create(int num_of_workers) {
  /* TODO malloc fail case */
  ChildData * c = malloc(num_of_workers * sizeof(ChildData));
  for (int i = 0; i < num_of_workers; i++) {
    c[i].is_working = false;
    c[i].semaphore = 0;
    c[i].shmem = NULL;  /* TODO create segment */
    c[i].current_file_segment = 0;
  }
  return c;
}

void child_data_free(ChildData *c) {
  free(c);  /* TODO clear memory */
}


