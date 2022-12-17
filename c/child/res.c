#include <stdlib.h>

#include "../both/dev_mode.h"
#include "res.h"


void child_res_init(ChildRes *res) {

  WELL("");
  res->application_time_in_ns = -1;
  res->responce_time_in_ns = -1;
  res->file_segment = -1;
  res->line_in_segment = -1;
  res->line_contents[0] = '\0';
}


int child_res_to_ostream(ChildRes *res, FILE *file) {

#define ARGS() "req: %u ns, res: %u ns, <%d,%d> $%s\n", \
    res->application_time_in_ns, \
    res->responce_time_in_ns, \
    res->file_segment, \
    res->line_in_segment, \
    res->line_contents

#ifdef DEV
  char *to_write;
  to_write = malloc(1024);
  sprintf(to_write, ARGS());
  WELL("");
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


