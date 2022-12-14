#include <stdio.h>

#include "dev_mode.h"
#include "params.h"
#include "fork.h"
#include "shmem.h"


int args_want_early_quit(Params *p);

int main(int argc, char **argv) {
  Params p;
  int err;

  WELL("parsing parameters");
  parameters_parse(&p, argc, argv);
  if (args_want_early_quit(&p))
    return 0;

  err = handle_forks(&p);

  WELL("DONE");
  return err;
}

int args_want_early_quit(Params *p) {

  if (p->show_help)
    parameters_help();

  else if (p->show_params)
    parameters_print(p);

  else
    return 0;

  return 1;
}
