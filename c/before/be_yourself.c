#include <unistd.h>

#include "be_yourself.h"
#include "../both/dev_mode.h"
#include "get_names.h"
#include "../parent/interface.h"
#include "../child/interface.h"

/* for README, testable-fork */
#ifndef TEST

int testable_fork() {
  return fork();
}
/* end of snippet */

int be_parent(Params *p, char **sem_names) {
  int err;
  Parent r;

  r.file_segment_length = p->file_segment_length;
  r.input_file = p->input_file;

  r.num_of_children = p->num_of_children;
  r.loops_per_child = p->loops_per_child;
  r.input_file = p->input_file;
  r.file_segment_length = p->file_segment_length;

  r.sem_name_yes_please = p->sem_name_i_want;
  r.shmem_name_yes_please = p->shmem_name_i_want;
  r.shmem_name_youre_ready = p->shmem_name_thank_you;

  r.lines_in_file = -1;
  r.sem_yes_please = NULL;
  r.sems_youre_ready = NULL;
  r.shmem_yes_please = NULL;
  r.shmem_youre_ready = NULL;

  err = parent_init(&r, p->parent_params, sem_names);
  if (err)
    return err;

  err = parent_loop(&r);
  WELL("freeing after parent_loop");
  parent_waitpid(&r);
  parent_free(&r);

  return err;
}

int be_child(Params *p, unsigned child_index, char *sem_name) {
  Child child;
  int err;

  WELL("");

  /* TODO change */
  child.sem_name_i_want = p->parent_params->sem_name_yes_please;
  child.sem_name_thank_you = sem_name;
  child.shmem_name_i_want = p->parent_params->shmem_name_yes_please;
  child.shmem_name_thank_you = p->parent_params->shmem_name_youre_ready;

  child.output_file = get_output_file_name(p->output_dir, child_index);
  child.file_segment_length = p->parent_params->file_segment_length;
  child.loops = p->parent_params->loops_per_child;
  child.microsecond_delay = p->microsecond_delay;

  child.id = child_index;
  child.num_of_children = p->parent_params->num_of_children;

  child.lines_in_file = -1;
  child.sem_i_want = NULL;
  child.sem_thank_you = NULL;
  child.shmem_i_want = NULL;
  child.shmem_thank_you = NULL;

  WELL(child.output_file);

  child_init(&child);
  err = child_loop(&child);
  child_free(&child);
  return err;
}

#endif

