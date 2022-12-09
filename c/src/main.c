#include <stdio.h>

#include "dev_mode.h"
#include "params.h"
#include "fork.h"
#include "shmem.h"


int main(int argc, char **argv) {
  Params *p;
  int err;
  void *shmem_i_want, *shmem_thank_you;

  WELL("parsing parameters");
  p = parameters_parse(argc, argv);

  if (p->show_help) {
    printf("well help you won't get\n");
    return 0;
  }

  WELL("creating shmem");
#include "defaults.h"
  shmem_i_want = shmem_create(DEFAULT_SHM_I_WANT, p->parent_params->file_segment_length);
  shmem_thank_you = shmem_create(DEFAULT_SHM_THANK_YOU, p->parent_params->file_segment_length);

  err = handle_forks(p, shmem_i_want, shmem_thank_you);

  parameters_free(p);
  shmem_free(DEFAULT_SHM_I_WANT, shmem_i_want);
  shmem_free(DEFAULT_SHM_THANK_YOU, shmem_thank_you);

  WELL("DONE");
  return err;
}

