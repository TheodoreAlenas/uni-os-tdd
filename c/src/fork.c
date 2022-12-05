#include <unistd.h>
#include <stdio.h>

#include "fork.h"
#include "parent.h"
#include "child.h"

#define SEC 1000000

int be_parent(Params *p);
int be_child(int id);

int handle_forks(Params *p) {
  int is_parent, child_id;

  for (child_id = 0; child_id < p->num_of_children; child_id++) {

    is_parent = fork();

    if (is_parent == -1) {
      perror("Fork failed: ");
      return 1;
    }

    if (is_parent)
      continue;
    else
      return be_child(child_id);
  }
  return be_parent(p);
}

int be_parent(Params *p) {
  int err;
  Parent *r;
  usleep(1 * SEC);
  r = parent_create(p->num_of_children, p->file_name, p->file_segment_length);
  err = parent_loop(r);
  parent_free(r);
  printf("[parent] Done\n");
  return err;
}

int be_child(int id) {
  usleep(0.2 * SEC);
  child_res_to_file(child_res_create(), "/tmp/rlr_out");  /* TODO not final */
  printf("[child %d] Done\n", id);
  return 0;
}
