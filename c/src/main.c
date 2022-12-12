#include <stdio.h>

#include "dev_mode.h"
#include "params.h"
#include "fork.h"
#include "shmem.h"


int args_want_early_quit(Params *p);

int main(int argc, char **argv) {
  Params *p;
  int err;

  WELL("parsing parameters");
  p = parameters_parse(argc, argv);
  if (args_want_early_quit(p))
    return 0;

  err = handle_forks(p);

  parameters_free(p);

  WELL("DONE");
  return err;
}

int args_want_early_quit(Params *p) {

  if (p->show_help) {
    printf("well help you won't get\n");
    return 1;
  }

  if (p->show_params) {
    parameters_print(p);
    return 1;
  }

  return 0;
}
