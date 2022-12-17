#include <unistd.h>
#include <stdio.h>

#include "both/dev_mode.h"
#include "before/fork.h"


int args_want_early_quit(Params *p);

int main(int argc, char **argv) {
  Params p;
  int err;
  pid_t pid;

  WELL("parsing parameters");
  parameters_parse(&p, argc, argv);
  if (args_want_early_quit(&p))
    return 0;

  pid = getpid();
  err = handle_forks(&p);
  if (getpid() == pid)
    printf("The output is in the %s directory\n",
        p.output_dir);

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
