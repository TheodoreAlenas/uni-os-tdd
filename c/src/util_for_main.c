#include "util_for_main.h"

Params *parameters_create() {
  Params *p = malloc(sizeof(Params));
  p->file_name = malloc(sizeof(char) * MAX_FILE_NAME_LEN);
  p->file_segment_length = 128;
  p->num_of_workers = 16;
  p->operations_of_each_worker = 1024;
  p->show_help = false;
  return p;
}

#define ITS(s) strcmp(argv[i], (s)) == 0

Params *parameters_parse(int argc, char **argv) {
  int i;
  struct parameters *p;
  p = parameters_create();

  for (i = 0; i < argc; i++) {

    if (ITS("--help")) {
      p->show_help = true;
    }
  }

  return p;
}

void parameters_free(Params *p) {
  free(p->file_name);
  free(p);
}
