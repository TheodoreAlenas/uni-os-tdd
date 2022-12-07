#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "child.h"
#include "dev_mode.h"
#include "child_res.h"

Child *child_create(ChildArgs args) {
  Child *child;
  child = malloc(sizeof(Child));
  child->file_name = args.file_name;

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  if (child->file_name)
    free(child->file_name);

  free(child);
}

int child_loop(Child *child) {
  ChildRes *res;

  WELL("");
  res = child_res_create();
  WELL("responce created, putting into file");
  res->file_segment = 1;
  res->line_in_segment = 2;
  res->application_time_in_ns = 3;
  res->responce_time_in_ns = 4;
  res->line_contents = "hello there";
  child_res_to_file(res, child->file_name);
  WELL("responce put in file");
  child_res_free(res);

  return 0;
}

