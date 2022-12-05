#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "shmem.h"
#include "parent.h"
#include "child.h"

/* nanoseconds per second, for the usleep function */
#define SEC 1000000

int be_parent(Params *p, void *shmem);
int be_child(int id, void *shmem);

int handle_forks(Params *p) {
  int is_parent, child_id;
  void *shmem;

  shmem = shmem_create(p->file_segment_length);

  for (child_id = 0; child_id < p->num_of_children; child_id++) {

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
  return be_parent(p, shmem);
}

int be_parent(Params *p, void *shmem) {
  int err;
  Parent *r;

  shmem_test_fill(shmem);
  usleep(1 * SEC);
  r = parent_create(p->num_of_children, p->file_name, p->file_segment_length);
  err = parent_loop(r);
  parent_free(r);
  printf("[parent] Done\n");
  return err;
}

int be_child(int id, void *shmem) {
  usleep(0.2 * SEC);
  printf("[child %d] %s\n", id, (char *) shmem);
  child_res_to_file(child_res_create(), "/tmp/rlr_out");  /* TODO not final */
  printf("[child %d] Done\n", id);
  return 0;
}
