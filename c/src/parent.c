#include "parent.h"

Parent *parent_create(int num_of_workers) {
  Parent *r = malloc(sizeof(Parent));
  r->children = child_data_create(num_of_workers);
  r->requests = stack_create(num_of_workers);
  return r;
}

void parent_free(Parent *r) {
  stack_free(r->requests);
  child_data_free(r->children);
  free(r);
}

int parent_loop(Parent *r) {
  printf("hello\n");
#ifdef DEV
  printf("dev is defined\n");
#endif
  return 0;
}

