#include <stdio.h>

#include "parent.h"



int main() {
  int num_of_workers = 10;
  int main_ret, free_ret;

  Parent *r = parent_create(num_of_workers);
  main_ret = parent_loop(r);
  parent_free(r);

  return main_ret;
}

