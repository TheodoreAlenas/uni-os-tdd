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


int be_parent(Params *p, void *shmem);
int be_child(int child_index, pid_t *set_me, pid_t to_this);

int handle_forks(Params *p, void *shmem) {
  pid_t pid, is_parent;
  int child_index, num_of_children;
  char *output_file_name, *childs_sem_name;
  ChildData *children;

  num_of_children = p->parent_params->num_of_children;  /* alias */

  children = child_data_malloc(num_of_children);
  if (children == NULL)
    return -1;

  for (child_index = 0; child_index < num_of_children; child_index++) {
    output_file_name = get_output_file_name(p->output_dir, child_index);

    pid = is_parent = fork();

    if (is_parent == -1) {
      perror("Fork failed: ");
      return 1;
    }

    if (is_parent)
      child_data_create(children + child_index, get_semaphore_name(child_index));
    else
      return be_child(child_index, &children[child_index].pid, pid);
    /* TODO free the output_file name */
  }
  WELL("forks done");
  p->parent_params->children = children;
  return be_parent(p, shmem);
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

int be_child(int child_index, pid_t *set_me, pid_t to_this) {
  Child *child;
  ChildArgs args;

  WELL("");

  args.sem_name_i_want = SEM_I_WANT;
  args.sem_name_thank_you = get_semaphore_name(child_index);
  args.shmem_name_i_want = SHM_I_WANT;
  args.shmem_name_thank_you = SHM_THANK_YOU;
  args.file_name = get_output_file_name(DEFAULT_OUTPUT_DIR, child_index);

  *set_me = to_this;

  WELL(args.file_name);

  child = child_create(args);
  child_loop(child);
  child_free(child);

  return 0;
}
