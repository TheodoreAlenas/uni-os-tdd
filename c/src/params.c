#include <stdlib.h>

#include "params.h"

Params *parameters_create() {
  Params *p = malloc(sizeof(Params));
  p->file_name = malloc(sizeof(char) * MAX_FILE_NAME_LEN);
  p->file_segment_length = 128;
  p->num_of_workers = 16;
  p->operations_of_each_worker = 1024;
  p->show_help = false;
  return p;
}

#define IF_ITS(s) else if (strcmp(argv[i], (s)) == 0)

Params *parameters_parse(int argc, char **argv) {
  int i;
  struct parameters *p;
  p = parameters_create();

  for (i = 1; i < argc; i++) {

    if (0) {}
    IF_ITS("--help")
      p->show_help = true;
    IF_ITS("-c")
      p->num_of_workers = atoi(argv[i+1]);
    else
      p->file_name = argv[i];
  }

  return p;
}
#undef IF_ITS

void parameters_free(Params *p) {
  free(p->file_name);
  free(p);
}
