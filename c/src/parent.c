#include <stdio.h>
#include <string.h>

#include "params.h"
#include "parent.h"
#include "parent_params.h"

char *read_segment_from_open_file(Parent *parent, FILE *file, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char **to_return, FILE *file);

Parent *parent_create(ParentParams *pp) {
  Parent *r = malloc(sizeof(Parent));

  r->pp = pp;

  r->children = child_data_create(r->pp->num_of_children);
  r->requests = stack_create(r->pp->num_of_children);

  return r;
}

void parent_free(Parent *r) {
  stack_free(r->requests);
  child_data_free(r->children);
  /* ATTENTION the ParentParams might be freed from elsewhere */
  free(r);
}

int parent_loop(Parent *r) {

  char *segment = parent_read_file_segment(r, 1);
  /* printf("%s\n", segment); */
  free(segment);

#ifdef DEV
  printf("dev is defined\n");
#endif

  return 0;
}

char *parent_read_file_segment(Parent *parent, unsigned long segment) {

  FILE *file;
  char *res;

  file = fopen(parent->pp->file_name, "r");
  res = read_segment_from_open_file(parent, file, segment);
  fclose(file);

  return res;
}

char *read_segment_from_open_file(Parent *parent, FILE *file, unsigned long segment) {
  int i, err;
  char *to_return;

  to_return = malloc(1024 * sizeof(char));
  if (to_return == NULL) {
    perror("malloc failed on reading string: ");
    return NULL;
  }

  err = skip_to_segment(file, segment, parent->pp->file_segment_length);
  if (err)
    return NULL;

  for (i = 0; i < parent->pp->file_segment_length; i++)
    append_to_final(&to_return, file);

  return to_return;
}

int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length) {
  int i;
  unsigned long starting_point;
  size_t length;
  ssize_t length_of_read;
  char *line = NULL;

  starting_point = segment * segment_length;

  for (i = 0; i < starting_point; i++)
    length_of_read = getline(&line, &length, file);

  if (line)
    free(line);

  if (length_of_read == -1)
    return -1;
  return 0;
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
