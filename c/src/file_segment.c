#include <string.h>
#include "file_segment.h"
#include "dev_mode.h"
#include "constants.h"

int read_segment_from_open_file(const ParentParams *pp, FILE *file, char *buf, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char *to_return, FILE *file);

int testable_read_file_segment(const Parent *parent, char *buf, unsigned long segment) {

  FILE *file;
  int err;

  file = fopen(parent->pp->file_name, "r");
  err = read_segment_from_open_file(parent->pp, file, buf, segment);
  fclose(file);

  WELLL(printf("segment: %lu, sample: %c%c", segment, buf[0], buf[1]));

  return err;
}

int read_segment_from_open_file(const ParentParams *pp, FILE *file, char *buf, unsigned long segment) {
  int i, err;
  char *to_return;

  err = skip_to_segment(file, segment, pp->file_segment_length);
  if (err)
    return -1;

  buf[0] = '\0';

  for (i = 0; i < pp->file_segment_length; i++)
    append_to_final(buf, file);

  return 0;
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

void append_to_final(char *to_return, FILE *file) {
  char *line = NULL;
  size_t length;
  ssize_t length_of_read;

  length_of_read = getline(&line, &length, file);

  if (length_of_read == -1)
    return;

  strcat(to_return, line);

  if (line)
    free(line);
}

