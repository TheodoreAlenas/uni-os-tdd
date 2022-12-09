#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "constants.h"
#include "defaults.h"
#include "dev_mode.h"
#include "get_names.h"
#include "shmem.h"
#include "parent.h"
#include "child.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000


int give_birth(Params *p, unsigned child_index, ChildData *children);
int be_parent(Params *p, void *shmem);
int be_child(Params *p, unsigned child_index);

int handle_forks(Params *p, void *shmem) {
  unsigned child_index, num_of_children;
  ChildData *children;

  num_of_children = p->parent_params->num_of_children;  /* alias */

  children = child_data_malloc(num_of_children);
  if (children == NULL)
    return -1;

  for (child_index = 0; child_index < num_of_children; child_index++)
    if (give_birth(p, child_index, children) != 0)
      return -1;

  WELL("forks done");
  p->parent_params->children = children;
  return be_parent(p, shmem);
}

int give_birth(Params *p, unsigned child_index, ChildData *children) {

  pid_t pid, is_parent;
  char *output_file_name, *childs_sem_name;

  output_file_name = get_output_file_name(p->output_dir, child_index);

  pid = is_parent = fork();

  if (pid == -1) {
    perror("Fork failed: ");
    return 1;
  }

  if (is_parent)
    child_data_create(children + child_index, get_semaphore_name(child_index));
  else {
    children[child_index].pid = pid;
    return be_child(p, child_index);
  }
  /* TODO free the output_file name */

  return 0;
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

int be_child(Params *p, unsigned child_index) {
  Child *child;
  ChildArgs args;

  WELL("");

  args.sem_name_i_want = p->parent_params->sem_name_yes_please;
  args.sem_name_thank_you = get_semaphore_name(child_index);
  args.shmem_name_i_want = p->parent_params->shmem_name_yes_please;
  args.shmem_name_thank_you = p->parent_params->shmem_name_youre_ready;
  args.file_name = get_output_file_name(p->output_dir, child_index);

  WELL(args.file_name);

  child = child_create(args);
  child_loop(child);
  child_free(child);

  return 0;
}

