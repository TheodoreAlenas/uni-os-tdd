#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parent.h"
#include "dev_mode.h"
#include "params.h"
#include "parent_params.h"

sem_t *init_sem_and_broadcast(const Parent *r);
char *read_segment_from_open_file(const ParentParams *pp, FILE *file, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char **to_return, FILE *file);

Parent *parent_create(const ParentParams *pp) {
  int i;
  Parent *r = malloc(sizeof(Parent));

  WELL("");
  r->pp = pp;
  r->requests = stack_create(r->pp->num_of_children);
  r->sem_yes_please = init_sem_and_broadcast(r);
  if (r->sem_yes_please == NULL)
    return NULL;

  return r;
}

sem_t *init_sem_and_broadcast(const Parent *r) {
  sem_t *s;
  unsigned i;

  s = sem_open(r->pp->sem_name_yes_please, O_CREAT | O_WRONLY, 0666, 0);
  if (s == NULL) {
    perror("parent's 'yes please' semaphore");
    return NULL;
  }

  WELL("signaling that the semaphore is ready");
  for (i = 0; i < r->pp->num_of_children; i++) {
    sem_post(r->pp->children[i].semaphore);
  }

  return s;
}

void parent_free(Parent *r) {
  WELL("(not freeing ParentParams)");
  stack_free(r->requests);
  if (r->sem_yes_please) {
    sem_unlink(r->pp->sem_name_yes_please);
    sem_close(r->sem_yes_please);
  }
  /* ATTENTION the ParentParams should be freed elsewhere */
  free(r);
}

int parent_loop(Parent *r) {
  sem_t *send_me, *wait_me;
  int i, j;

  for (j = 0; j < 3; j++) {
    for (i = 0; i < r->pp->num_of_children; i++) {
      WELL("waiting for anyone to ask something");
      sem_wait(r->sem_yes_please);
      sem_post(r->pp->children[i].semaphore);
      sem_wait(r->sem_yes_please);

      char *segment = parent_read_file_segment(r, 1);
      /* printf("%s\n", segment); */
      /* TODO shmem */
      free(segment);

      WELLL(printf("telling child #%d that its file segment is ready", i));
      sem_post(r->pp->children[i].semaphore);
    }
  }
  WELL("loop done");

  return 0;
}

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

int parent_waitpid(const Parent *r) {
  int i, status;

  for (i = 0; i < r->pp->num_of_children; i++) {
    WELLL(printf("waiting pid %d", r->pp->children[i].pid));
    waitpid(r->pp->children[i].pid, &status, 0);
  }

  if (WEXITSTATUS(status))
    return 0;
  return -1;
}
