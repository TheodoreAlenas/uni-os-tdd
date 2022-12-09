#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "constants.h"
#include "dev_mode.h"
#include "get_names.h"
#include "shmem.h"
#include "parent.h"
#include "child.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000


int give_birth(Params *p, void *shmem, ChildData *children);
void store_child_for_parent(ChildData *child, char *sem_name, pid_t pid);
int case_child(Params *p, unsigned child_index, char *sem_name);
int be_parent(Params *p, void *shmem);
int be_child(Params *p, unsigned child_index, char *sem_name);

int handle_forks(Params *p, void *shmem) {
  unsigned n;
  ChildData *children;
  int err;

  n = p->parent_params->num_of_children;  /* copying for safety */

  children = child_data_malloc(n);
  if (children == NULL)
    return -1;

  err = give_birth(p, shmem, children);

  child_data_free_all(children, n);
  return err;
}

int give_birth(Params *p, void *shmem, ChildData *children) {
  unsigned child_index;
  pid_t pid, is_parent;
  char *sem_name;

  for (child_index = 0; child_index < p->parent_params->num_of_children; child_index++) {
    sem_name = get_semaphore_name(child_index);
    pid = is_parent = fork();

    if (pid > 0)
      store_child_for_parent(children + child_index, sem_name, pid);

    else if (pid == 0)
      return case_child(p, child_index, sem_name);

    else {
      perror("fork failed");
      return 1;
    }

    if (sem_name)
      free(sem_name);
  }
  WELL("forks done");
  p->parent_params->children = children;
  return be_parent(p, shmem);
}

void store_child_for_parent(ChildData *child, char *sem_name, pid_t pid) {
  child_data_create(child, sem_name);
  child->pid = pid;  /* the other attributes are initialized */
}

int case_child(Params *p, unsigned child_index, char *sem_name) {
  int err;

  err = be_child(p, child_index, sem_name);

  if (sem_name)
    free(sem_name);

  return err;
}

int be_parent(Params *p, void *shmem) {
  int err;
  Parent *r;

  shmem_test_fill(shmem);
  r = parent_create(p->parent_params);
  err = parent_loop(r);
  WELL("freeing after parent_loop");
  parent_waitpid(r);
  parent_free(r);
  return err;
}

int be_child(Params *p, unsigned child_index, char *sem_name) {
  Child *child;
  ChildArgs *args;

  WELL("");

  args = malloc(sizeof(ChildArgs));
  args->sem_name_i_want = p->parent_params->sem_name_yes_please;
  args->sem_name_thank_you = sem_name;
  args->shmem_name_i_want = p->parent_params->shmem_name_yes_please;
  args->shmem_name_thank_you = p->parent_params->shmem_name_youre_ready;
  args->file_name = get_output_file_name(p->output_dir, child_index);

  child = child_create(args);
  child_loop(child);
  child_free(child);

  free(args);

  return 0;
}

