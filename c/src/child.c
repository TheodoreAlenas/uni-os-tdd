#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "child.h"

ChildRes *child_res_create() {
  ChildRes *res = malloc(sizeof(ChildRes));

  res->application_time_in_ns = -1;
  res->responce_time_in_ns = -1;
  res->file_segment = -1;
  res->line_in_segment = -1;
  res->line_contents = NULL;

  return res;
}

void child_res_free(ChildRes *res) {
  if (res->line_contents != NULL)
    free(res->line_contents);
  free(res);
}

int child_res_to_ostream(ChildRes *res, FILE *file) {

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

