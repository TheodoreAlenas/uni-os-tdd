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
#include "shmem.h"

Child *try_opening_sem_i_want(Child *child, const ChildArgs *args);
void do_a_cycle(const Child *c);

Child *child_create(const ChildArgs *args) {
  Child *child;

  child = malloc(sizeof(Child));

  child->names = args;
  WELLL(printf("file name: %s", child->names->file_name));

  child->sem_thank_you = sem_open(args->sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return NULL;
  }

  WELL("waiting for parent to create his semaphore");
  sem_wait(child->sem_thank_you);

  /* TODO look at the output, there's an odd coorelation here. */
  child->shmem_i_want    = shmem_open_write_only(args->shmem_name_i_want, 1);
  child->shmem_thank_you = shmem_open_read_only(args->shmem_name_thank_you, 1);
  //void *another = shmem_open_write_only(args.shmem_name_i_want, 1);

  child = try_opening_sem_i_want(child, args);

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  sem_unlink(child->names->sem_name_thank_you);
  sem_close(child->sem_i_want);
  sem_close(child->sem_thank_you);
  shmem_free(child->names->shmem_name_i_want);
  shmem_free(child->names->shmem_name_thank_you);

  free(child);
}

Child *try_opening_sem_i_want(Child *child, const ChildArgs *args) {

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

void child_loop(const Child *child) {
  int i;

  WELLL(printf("%s", child->names->file_name));

  for (i = 0; i < child->names->loops; i++)
    do_a_cycle(child);

  WELL("loop done");
}

void do_a_cycle(const Child *child) {
  ChildRes *res;

  WELL("asking to ask");
  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);

  WELL("asking, with details");
  sprintf(child->shmem_i_want, "yo mum");
  //WELL("the detaile came...");
  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);

  WELL("parent says I can read");
  WELL(child->names->file_name);

  res = child_res_create();
  res->file_segment = 1;
  res->line_in_segment = 2;
  res->application_time_in_ns = 3;
  res->responce_time_in_ns = 4;
  res->line_contents = malloc(MAX_LINE_LEN);
  strcpy(res->line_contents, child->shmem_thank_you);
  child_res_to_file(res, child->names->file_name);
  child_res_free(res);

  WELL("responce put in file");
  usleep(200000);
  WELL("done");
}
