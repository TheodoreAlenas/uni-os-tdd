#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "parent.h"

int handle_forks(Params *p) {
  int parent_err;
  Parent *r = parent_create(p->num_of_workers);
  parent_err = parent_loop(r);
  parent_free(r);
  printf("[handle_forks] Hi!\n");
  return 0;
}

