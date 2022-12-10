#include <string.h>
#include "file_segment.h"
#include "dev_mode.h"

char *read_segment_from_open_file(const ParentParams *pp, FILE *file, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char **to_return, FILE *file);

char *parent_read_file_segment(const Parent *parent, unsigned long segment) {

  FILE *file;
  char *res;

  WELLL(printf("%lu", segment));
  file = fopen(parent->pp->file_name, "r");
  res = read_segment_from_open_file(parent->pp, file, segment);
  fclose(file);

  return res;
}

char *read_segment_from_open_file(const ParentParams *pp, FILE *file, unsigned long segment) {
  int i, err;
  char *to_return;

  WELL("");
  to_return = malloc(1024 * sizeof(char));
  if (to_return == NULL) {
    perror("malloc failed on reading string: ");
    return NULL;
  }

  err = skip_to_segment(file, segment, pp->file_segment_length);
  if (err)
    return NULL;

  for (i = 0; i < pp->file_segment_length; i++)
    append_to_final(&to_return, file);

  WELL("Done");
  return to_return;
}

int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length) {
  int i;
  unsigned long start_of_segment;
  size_t length;
  ssize_t length_of_read;
  char *garbage_line = NULL;

  start_of_segment = segment * segment_length;

  for (i = 0; i < start_of_segment; i++)
    length_of_read = getline(&garbage_line, &length, file);

  if (garbage_line)
    free(garbage_line);

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

