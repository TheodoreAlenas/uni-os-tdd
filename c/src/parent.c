#include <stdio.h>
#include <string.h>

#include "params.h"
#include "parent.h"

char *read_segment_from_open_file(Parent *parent, FILE *file, unsigned long segment);
void append_to_final(char **to_return, FILE *file);

Parent *parent_create(int num_of_children, char *file_name, unsigned long file_segment_length) {
  Parent *r = malloc(sizeof(Parent));

  r->children = child_data_create(num_of_children);
  r->requests = stack_create(num_of_children);
  r->file_name = malloc(MAX_FILE_NAME_LEN * sizeof(char));
  strcpy(r->file_name, file_name);

  r->num_of_children = num_of_children;
  r->file_segment_length = file_segment_length;
  return r;
}

void parent_free(Parent *r) {
  stack_free(r->requests);
  child_data_free(r->children);
  free(r->file_name);
  free(r);
}

int parent_loop(Parent *r) {


  fprintf(fopen("/dev/null", "w"), "%s\n", parent_read_file_segment(r, 1));

#ifdef DEV
  printf("dev is defined\n");
#endif

  return 0;
}

char *parent_read_file_segment(Parent *parent, unsigned long segment) {

  FILE *file;
  char *res;

  file = fopen(parent->file_name, "r");
  res = read_segment_from_open_file(parent, file, segment);
  fclose(file);

  return res;
}

char *read_segment_from_open_file(Parent *parent, FILE *file, unsigned long segment) {
  int i;
  size_t length;
  unsigned long starting_point;
  char *line = NULL, *to_return;

  to_return = malloc(1024 * sizeof(char));
  if (to_return == NULL) {
    perror("malloc failed on reading string: ");
    return NULL;
  }

  starting_point = segment * parent->file_segment_length;

  for (i = 0; i < starting_point; i++)
    getline(&line, &length, file);

  for (i = 0; i < parent->file_segment_length; i++)
    append_to_final(&to_return, file);

  if (line)
    free(line);

  return to_return;
}

void append_to_final(char **to_return, FILE *file) {
  char *line = NULL;
  size_t length;
  ssize_t length_of_read;

  length_of_read = getline(&line, &length, file);

  if (length_of_read == -1)
    return;

  strcat(*to_return, line);

  if (line)
    free(line);
}
