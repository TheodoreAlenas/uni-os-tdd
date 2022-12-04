#include <stdio.h>

#include "stack.h"


typedef struct per_child {
  int semaphore;
  void *shmem;
  bool is_working;
  int current_file_segment;
} PerChild;

int child_data_create(int num_of_workers);

typedef struct main_resources {
  Stack *requests;
  PerChild *children;
} MainResources;

MainResources *create_resources_of_main(int num_of_workers);
int main_loop(MainResources *r);
int free_resources_of_main(MainResources *r);

int do_the_rest(int num_of_workers);


int main() {
  int num_of_workers = 10;
  int main_ret, free_ret;

  MainResources *r = create_resources_of_main(num_of_workers);
  main_ret = main_loop(r);
  free_ret = free_resources_of_main(r);

  if (main_ret != 0)
    return main_ret;
  if (free_ret != 0)
    return free_ret;
  return 0;
}


int do_the_rest(int num_of_workers) {
  return 0;
}


MainResources *create_resources_of_main(int num_of_workers) {
  PerChild *children = malloc(num_of_workers * sizeof(PerChild));
  Stack *requests = stack_create(num_of_workers);
  MainResources *r = malloc(num_of_workers * sizeof(MainResources));
  return r;
}

int main_loop(MainResources *r) {
  printf("hello\n");
#ifdef DEV
  printf("dev is defined\n");
#endif
  return 0;
}

int free_resources_of_main(MainResources *r) {
  return 0;
}
