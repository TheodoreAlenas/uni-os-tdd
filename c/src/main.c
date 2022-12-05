#include <stdio.h>

#include "params.h"
#include "fork.h"


#define TRY(n) if ((n)) { perror( #n " failed: " ); parameters_free(p); return -1; }

int main(int argc, char **argv) {
  Params *p = parameters_parse(argc, argv);

  if (p->show_help) {
    printf("well help you won't get\n");
  }

  TRY(handle_forks(p));

  return 0;
}

