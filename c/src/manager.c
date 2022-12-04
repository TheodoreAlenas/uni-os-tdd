#include <stdio.h>

#include "stack.h"


typedef struct child_data {
  int semaphore;
  void *shmem;
  bool is_working;
  int current_file_segment;
} ChildData;

ChildData *child_data_create(int num_of_workers);
void child_data_free(ChildData *c);

typedef struct main_resources {
  Stack *requests;
  ChildData *children;
} MainResources;

MainResources *create_resources_of_main(int num_of_workers);
int main_loop(MainResources *r);
void free_resources_of_main(MainResources *r);

int do_the_rest(int num_of_workers);


int main() {
  int num_of_workers = 10;
  int main_ret, free_ret;

  MainResources *r = create_resources_of_main(num_of_workers);
  main_ret = main_loop(r);
  free_resources_of_main(r);

  return main_ret;
}


int do_the_rest(int num_of_workers) {
  return 0;
}


MainResources *create_resources_of_main(int num_of_workers) {
  MainResources *r = malloc(sizeof(MainResources));
  r->children = child_data_create(num_of_workers);
  r->requests = stack_create(num_of_workers);
  return r;
}

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

void free_resources_of_main(MainResources *r) {
  stack_free(r->requests);
  child_data_free(r->children);
  free(r);
}

int main_loop(MainResources *r) {
  printf("hello\n");
#ifdef DEV
  printf("dev is defined\n");
#endif
  return 0;
}

