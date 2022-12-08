#include <stdlib.h>

#include "dev_mode.h"
#include "child_res.h"


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
  WELL(res->line_contents);
  WELL("screw it");  /* TODO */
  return;
  if (res->line_contents != NULL)
    free(res->line_contents);
  WELL("done with the line contents");
  free(res);
  WELL("done with everything else too");
}

int child_res_to_ostream(ChildRes *res, FILE *file) {

#define ARGS() "%d %d <%d,%d> %s\n", \
    res->application_time_in_ns, \
    res->responce_time_in_ns, \
    res->file_segment, \
    res->line_in_segment, \
    res->line_contents

#ifdef DEV
  char *to_write;
  to_write = malloc(1024);
  sprintf(to_write, ARGS());
  WELL(to_write);
  return fprintf(file, to_write);
#else

  return fprintf(file, ARGS());

#endif
#undef ARGS
}

int child_res_to_file(ChildRes *res, char *filename) {
  int err;
  FILE *file;

  WELL(filename);

  file = fopen(filename, "a");

  if (file == NULL) {
    file = fopen(filename, "w");

    if (file == NULL) {
      perror("child opening file for appending");
      return -1;
    }
  }

  err = child_res_to_ostream(res, file);
  if (err == -1) {
    perror(__FUNCTION__);
    return -1;
  }

  fclose(file);

  return 0;
}


