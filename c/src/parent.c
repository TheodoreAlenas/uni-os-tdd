#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#include "parent.h"
#include "defaults.h"
#include "dev_mode.h"
#include "params.h"
#include "parent_params.h"

char *read_segment_from_open_file(ParentParams *pp, FILE *file, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char **to_return, FILE *file);

Parent *parent_create(ParentParams *pp) {
  Parent *r = malloc(sizeof(Parent));

  WELL("");
  r->pp = pp;

  r->children = child_data_create(r->pp->num_of_children);
  r->requests = stack_create(r->pp->num_of_children);

  return r;
}

void parent_free(Parent *r) {
  WELL("(not freeing ParentParams)");
  stack_free(r->requests);
  child_data_free(r->children);
  /* ATTENTION the ParentParams should be freed elsewhere */
  free(r);
}

int parent_loop(Parent *r) {
  sem_t *send_me, *wait_me;
  int i;

  WELL("waiting");
/*
  r->children[0].semaphore = sem_open("sem0", O_CREAT | O_RDONLY, 0666, 0);
  r->children[1].semaphore = sem_open("sem1", O_CREAT | O_RDONLY, 0666, 0);
  */

  send_me = sem_open(SEM_THANK_YOU, O_WRONLY, 0666, 0);

  for (i = 0; i < r->pp->num_of_children; i++) {
    sem_wait(r->children[i].semaphore);
    sem_post(send_me);
  }

  WELL("posted");

  char *segment = parent_read_file_segment(r, 1);
  /* printf("%s\n", segment); */
  free(segment);

  return 0;
}

char *parent_read_file_segment(Parent *parent, unsigned long segment) {

  FILE *file;
  char *res;

  WELLL(printf("%lu", segment));
  file = fopen(parent->pp->file_name, "r");
  res = read_segment_from_open_file(parent->pp, file, segment);
  fclose(file);

  return res;
}

char *read_segment_from_open_file(ParentParams *pp, FILE *file, unsigned long segment) {
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
