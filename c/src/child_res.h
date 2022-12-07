#ifndef CHILD_RES_H
#define CHILD_RES_H

#include <stdio.h>

typedef struct child_responce {
  int application_time_in_ns;
  int responce_time_in_ns;
  int file_segment;
  int line_in_segment;
  char *line_contents;
} ChildRes;

ChildRes *child_res_create();
void child_res_free(ChildRes *res);
int child_res_to_ostream(ChildRes *res, FILE *file);
int child_res_to_file(ChildRes *res, char *filename);

#endif
