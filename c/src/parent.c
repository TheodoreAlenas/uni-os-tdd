#include "parent.h"

MainResources *create_resources_of_main(int num_of_workers) {
  MainResources *r = malloc(sizeof(MainResources));
  r->children = child_data_create(num_of_workers);
  r->requests = stack_create(num_of_workers);
  return r;
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


