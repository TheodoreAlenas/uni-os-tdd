#include <stdio.h>

#include "params.h"
#include "fork.h"
#include "shmem.h"


#define TRY(n) if ((n)) { perror( #n " failed: " ); parameters_free(p); return -1; }

int main(int argc, char **argv) {
  Params *p = parameters_parse(argc, argv);
  void *shmem;

  if (p->show_help) {
    printf("well help you won't get\n");
    return 0;
  }

  shmem = shmem_create(p->file_segment_length);

  TRY(handle_forks(p, shmem));

  parameters_free(p);
  shmem_free(shmem);

  return 0;
}

