#include <stdio.h>

#include "parent.h"



int main() {
  int num_of_workers = 10;
  int main_ret, free_ret;

  MainResources *r = create_resources_of_main(num_of_workers);
  main_ret = main_loop(r);
  free_resources_of_main(r);

  return main_ret;
}

