#include <unistd.h>

#include "be_yourself.h"
#include "dev_mode.h"
#include "get_names.h"
#include "parent.h"
#include "child.h"

/* for README, testable-fork */
#ifndef TEST

int testable_fork() {
  return fork();
}
/* end of snippet */

int be_parent(Params *p, char **sem_names) {
  int err;
  Parent *r;

  r = parent_create(p->parent_params, sem_names);
  err = parent_loop(r);
  WELL("freeing after parent_loop");
  parent_waitpid(r);
  parent_free(r);
  return err;
}

int be_child(Params *p, unsigned child_index, char *sem_name) {
  Child *child;
  ChildArgs args;

  WELL("");

  args.sem_name_i_want = p->parent_params->sem_name_yes_please;
  args.sem_name_thank_you = sem_name;
  args.shmem_name_i_want = p->parent_params->shmem_name_yes_please;
  args.shmem_name_thank_you = p->parent_params->shmem_name_youre_ready;
  args.file_name = get_output_file_name(p->output_dir, child_index);
  args.loops = p->parent_params->loops_per_child;
  args.file_segment_length = p->parent_params->file_segment_length;
  args.id = child_index;
  args.num_of_children = p->parent_params->num_of_children;

  WELL(args.file_name);

  child = child_create(&args);
  child_loop(child);  /* TODO returns err */
  child_free(child);

  return 0;
}

#endif

