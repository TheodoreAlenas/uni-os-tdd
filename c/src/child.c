#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#include "child.h"
#include "constants.h"
#include "dev_mode.h"
#include "child_res.h"

Child *try_opening_sem_i_want(Child *child, ChildArgs *args);
void do_a_cycle(Child *c);

Child *child_create(ChildArgs args) {
  Child *child;

  child = malloc(sizeof(Child));

  child->names = malloc(sizeof(ChildArgs));
  memcpy(child->names, &args, sizeof(ChildArgs));

  /* TODO shmem */

  WELL(args.sem_name_i_want);
  WELL(args.sem_name_thank_you);

  child->sem_thank_you = sem_open(args.sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return NULL;
  }

  WELL("waiting for parent to create his semaphore");
  sem_wait(child->sem_thank_you);

  child = try_opening_sem_i_want(child, &args);

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  if (child->names)
    free(child->names);

  sem_unlink(child->names->sem_name_thank_you);
  sem_close(child->sem_i_want);
  sem_close(child->sem_thank_you);

  free(child);
}

Child *try_opening_sem_i_want(Child *child, ChildArgs *args) {

  child->sem_i_want = sem_open(args->sem_name_i_want, O_WRONLY, 0666, 0);
  if (child->sem_i_want == NULL) {
    perror("child's 'I want' semaphore");
    sem_unlink(args->sem_name_thank_you);
    sem_close(child->sem_thank_you);
    return NULL;
  }
  WELL("created child's own semaphore");
  return child;
}

int child_loop(Child *child) {
  int j;

  for (j = 0; j < 3; j++)
    do_a_cycle(child);

  WELL("loop done");
  return 0;
}

void do_a_cycle(Child *child) {
  ChildRes *res;

  WELL("asking to read, using semaphore");
  WELL(child->names->sem_name_i_want);
  sem_post(child->sem_i_want);
  WELL("waiting for the file segment to come");

  sem_wait(child->sem_thank_you);
  WELL("parent says I can read");

  res = child_res_create();
  WELL("responce created, putting into file");
  res->file_segment = 1;
  res->line_in_segment = 2;
  res->application_time_in_ns = 3;
  res->responce_time_in_ns = 4;
  res->line_contents = malloc(MAX_LINE_LEN);
  strcpy(res->line_contents, "hello there");
  child_res_to_file(res, child->names->file_name);
  WELL("responce put in file");
  child_res_free(res);

  WELL("faking a time wasteful process");
  usleep(200000);
}
