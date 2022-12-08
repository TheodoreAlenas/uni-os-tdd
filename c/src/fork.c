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
int be_child(int child_index, char *output_file);

int handle_forks(Params *p, void *shmem) {
  int is_parent, child_index, num_of_children;
  char *output_file_name;

  num_of_children = p->parent_params->num_of_children;  /* alias */
  for (child_index = 0; child_index < num_of_children; child_index++) {
    output_file_name = get_output_file_name(p->output_dir, child_index);

    is_parent = fork();

    if (is_parent == -1) {
      perror("Fork failed: ");
      return 1;
    }

    if (is_parent)
      continue;
    else
      return be_child(child_index, output_file_name);
    /* TODO free the output_file name */
  }
  WELL("forks done");
  return be_parent(p, shmem);
}

int be_parent(Params *p, void *shmem) {
  int err;
  Parent *r;

  shmem_test_fill(shmem);
  usleep(0.5 * SEC);
  r = parent_create(p->parent_params);
  err = parent_loop(r);
  WELL("freeing after parent_loop");
  usleep(200000);
  /* TODO waitpid */
  parent_free(r);
  return err;
}

int be_child(int child_index, char *output_file) {
  Child *child;
  ChildArgs args;

  WELL("");
  usleep(0.1 * SEC);

  args.sem_name_i_want = SEM_I_WANT;
  args.sem_name_thank_you = get_semaphore_name(child_index);
  args.shmem_name_i_want = SHM_I_WANT;
  args.shmem_name_thank_you = SHM_THANK_YOU;
  args.file_name = output_file;

  WELL(args.file_name);

  child = child_create(args);
  child_loop(child);
  child_free(child);

  return 0;
}
