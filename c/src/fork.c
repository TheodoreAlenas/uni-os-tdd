#include <stdio.h>

#include "fork.h"
#include "constants.h"
#include "dev_mode.h"
#include "be_yourself.h"
#include "get_names.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000


int give_birth(Params *p);
int be_child_and_free(Params *p, unsigned child_index, char *sem_name);

int handle_forks(Params *p) {
  unsigned n;
  int err;

  n = p->parent_params->num_of_children;  /* copying for safety */

  err = give_birth(p);

  return err;
}

int give_birth(Params *p) {
  unsigned child_index;
  pid_t pid, is_parent;
  char *sem_name, **sem_names;

  sem_names = malloc(p->parent_params->num_of_children * sizeof(char*));

  for (child_index = 0; child_index < p->parent_params->num_of_children; child_index++) {
    sem_name = get_semaphore_name(child_index);
    sem_names[child_index] = sem_name;
    pid = is_parent = testable_fork();

    if (pid == 0)
      return be_child_and_free(p, child_index, sem_name);  /* TODO free sem_names */

    else if (pid < 0) {
      perror("fork failed");
      return 1;
    }
  }
  WELL("forks done");
  return be_parent(p, sem_names);
}

int be_child_and_free(Params *p, unsigned child_index, char *sem_name) {
  int err;

  err = be_child(p, child_index, sem_name);

  if (sem_name)
    free(sem_name);

  return err;
}

