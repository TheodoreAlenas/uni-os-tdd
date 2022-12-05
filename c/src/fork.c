#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "parent.h"

#define SEC 1000000

int be_parent(int num_of_workers);
int be_child(int id);

int handle_forks(Params *p) {
  int is_parent, child_id;

  for (child_id = 0; child_id < p->num_of_workers; child_id++) {

    is_parent = fork();

    if (is_parent == -1) {
      perror("Fork failed: ");
      return 1;
    }

    if (is_parent)
      continue;
    else
      return be_child(child_id);
  }
  return be_parent(p->num_of_workers);
}

int be_parent(int num_of_workers) {
  int err;
  Parent *r = parent_create(num_of_workers);
  usleep(1 * SEC);
  err = parent_loop(r);
  parent_free(r);
  printf("[parent] Hi!\n");
  return err;
}

int be_child(int id) {
  usleep(0.9 * SEC);
  printf("[child %d] Hi!\n", id);
  return 0;
}
