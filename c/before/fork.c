#include <stdio.h>

#include "fork.h"
#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "be_yourself.h"
#include "get_names.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000


int give_birth(Params *p);
int be_child_and_free(Params *p, unsigned child_index);

int handle_forks(Params *p) {
  unsigned n;
  int err;

  n = p->num_of_children;  /* copying for safety */

  err = give_birth(p);

  return err;
}

int give_birth(Params *p) {
  unsigned child_index;
  pid_t pid, is_parent;

  for (child_index = 0; child_index < p->num_of_children; child_index++) {
    pid = is_parent = testable_fork();

    if (pid == 0)
      return be_child_and_free(p, child_index);

    else if (pid < 0) {
      perror("fork failed");
      return 1;
    }
  }
  WELL("forks done");
  return be_parent(p);
}

int be_child_and_free(Params *p, unsigned child_index) {
  int err;

  /* TODO remove function */
  err = be_child(p, child_index);
  return err;
}

