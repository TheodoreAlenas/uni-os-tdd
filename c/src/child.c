#include <string.h>
#include <stdio.h>

#include "child.h"

int append_to_file(ChildRes *res, char *filename) {
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

int child_res_to_ostream(ChildRes *res, FILE *file) {

  return fprintf(file, "%d %d <%d,%d> %s",
      res->application_time_in_ns,
      res->responce_time_in_ns,
      res->file_segment,
      res->line_in_segment,
      res->line_contents
      );
}
