#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>

typedef struct child_responce {
  int application_time_in_ns;
  int responce_time_in_ns;
  int file_segment;
  int line_in_segment;
  char *line_contents;
} ChildRes;

int child_res_to_ostream(ChildRes *res, FILE *file);
int append_to_file(ChildRes *res, char *filename);  /* TODO test at all */

#endif
