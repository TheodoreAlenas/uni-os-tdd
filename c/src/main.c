#include <stdio.h>

#include "dev_mode.h"
#include "params.h"
#include "fork.h"
#include "shmem.h"


int main(int argc, char **argv) {
  Params *p;
  int err;
  void *shmem;

  I_AM("main"); WERE_IN("main");
  WELL("parsing parameters");
  p = parameters_parse(argc, argv);

  if (p->show_help) {
    printf("well help you won't get\n");
    return 0;
  }

  WELLL(printf("creating %d shmem", 1));
  shmem = shmem_create(p->parent_params->file_segment_length);

  err = handle_forks(p, shmem);

  parameters_free(p);
  shmem_free(shmem);

  return err;
}

