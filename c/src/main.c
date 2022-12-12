#include <stdio.h>

#include "dev_mode.h"
#include "arg_parse.h"
#include "fork.h"
#include "shmem.h"


int main(int argc, char **argv) {
  Params *p;
  int err;

  WELL("parsing parameters");
  p = parameters_parse(argc, argv);

  if (p->show_help) {
    printf("well help you won't get\n");
    return 0;
  }
  if (p->quit) {
    return 0;
  }

  WELL("creating shmem");

  err = handle_forks(p);

  parameters_free(p);

  WELL("DONE");
  return err;
}

