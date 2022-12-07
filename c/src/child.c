#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "child.h"
#include "dev_mode.h"
#include "child_res.h"

Child *child_create(ChildArgs args) {
  Child *child;
  child = malloc(sizeof(Child));

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  free(child);
}

int child_loop(Child *child) {
  WELL("");

  child_res_to_file(child_res_create(), "/tmp/rlr_out");  /* TODO not final */
  return 0;
}

