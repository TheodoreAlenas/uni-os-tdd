#include <stdio.h>

#include "util_for_main.h"
#include "parent.h"



int main(int argc, char **argv) {
  int main_ret, free_ret;
  Params *p = parameters_parse(argc, argv);

  if (p->show_help) {
    printf("well help you won't get\n");
  }
  Parent *r = parent_create(p->num_of_workers);
  main_ret = parent_loop(r);
  parent_free(r);

  return main_ret;
}

