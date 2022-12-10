#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "constants.h"
#include "dev_mode.h"
#include "be_yourself.h"
#include "get_names.h"
#include "shmem.h"
#include "parent.h"
#include "child.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000


int give_birth(Params *p, ChildData *children);
void store_child_for_parent(ChildData *child, char *sem_name, pid_t pid);
int be_child_and_free(Params *p, unsigned child_index, char *sem_name);

int handle_forks(Params *p) {
  unsigned n;
  ChildData *children;
  int err;

  n = p->parent_params->num_of_children;  /* copying for safety */

  children = child_data_malloc(n);
  if (children == NULL)
    return -1;

  err = give_birth(p, children);

  child_data_free_all(children, n);
  return err;
}

int give_birth(Params *p, ChildData *children) {
  unsigned child_index;
  pid_t pid, is_parent;
  char *sem_name;

  for (child_index = 0; child_index < p->parent_params->num_of_children; child_index++) {
    sem_name = get_semaphore_name(child_index);
    pid = is_parent = fork();

    if (pid > 0)
      store_child_for_parent(children + child_index, sem_name, pid);

    else if (pid == 0)
      return be_child_and_free(p, child_index, sem_name);

    else {
      perror("fork failed");
      return 1;
    }

    if (sem_name)
      free(sem_name);
  }
  WELL("forks done");
  return be_parent(p, children);
}

void store_child_for_parent(ChildData *child, char *sem_name, pid_t pid) {
  child_data_create(child, sem_name);
  child->pid = pid;  /* the other attributes are initialized */
}

int be_child_and_free(Params *p, unsigned child_index, char *sem_name) {
  int err;

  err = be_child(p, child_index, sem_name);

  if (sem_name)
    free(sem_name);

  return err;
}

