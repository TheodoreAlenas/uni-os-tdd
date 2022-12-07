#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "dev_mode.h"
#include "shmem.h"
#include "parent.h"
#include "child.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000

int be_parent(Params *p, void *shmem);
int be_child(int id, void *shmem);

int handle_forks(Params *p, void *shmem) {
  int is_parent, child_id;

  for (child_id = 0; child_id < p->parent_params->num_of_children; child_id++) {

    is_parent = fork();

    if (is_parent == -1) {
      perror("Fork failed: ");
      return 1;
    }

    if (is_parent)
      continue;
    else
      return be_child(child_id, shmem);
  }
  WELL("forks done");
  return be_parent(p, shmem);
}

int be_parent(Params *p, void *shmem) {
  int err;
  Parent *r;

  shmem_test_fill(shmem);
  usleep(0.3 * SEC);
  r = parent_create(p->parent_params);
  err = parent_loop(r);
  WELL("freeing after parent_loop");
  parent_free(r);
  return err;
}

int be_child(int id, void *shmem) {
  usleep(0.1 * SEC);
  WELLL(printf("shmem contains '%s'", (char *) shmem));
  child_res_to_file(child_res_create(), "/tmp/rlr_out");  /* TODO not final */
  WELL("Done");
  return 0;
}

char *get_semaphore_name(unsigned child_index) {
  char *ans;
  ans = malloc(256);
  sprintf(ans, "sem%u", child_index);
  return ans;
}
