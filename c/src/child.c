#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "child.h"
#include "dev_mode.h"

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

ChildRes *child_res_create() {
  ChildRes *res = malloc(sizeof(ChildRes));

  WELL("");
  res->application_time_in_ns = -1;
  res->responce_time_in_ns = -1;
  res->file_segment = -1;
  res->line_in_segment = -1;
  res->line_contents = NULL;

  return res;
}

void child_res_free(ChildRes *res) {
  WELL("");
  if (res->line_contents != NULL)
    free(res->line_contents);
  free(res);
}

int child_res_to_ostream(ChildRes *res, FILE *file) {

  WELL("");
  return fprintf(file, "%d %d <%d,%d> %s\n",
      res->application_time_in_ns,
      res->responce_time_in_ns,
      res->file_segment,
      res->line_in_segment,
      res->line_contents
      );
}

int child_res_to_file(ChildRes *res, char *filename) {
  int err;
  FILE *file = fopen(filename, "a");

  WELL(filename);
  if (file == NULL) {
    perror("Child opening file for appending: ");
    return -1;
  }

  err = child_res_to_ostream(res, file);
  if (err)
    return -1;

  fclose(file);

  return 0;
}

