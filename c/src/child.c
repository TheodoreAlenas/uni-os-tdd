#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#include "child.h"
#include "dev_mode.h"
#include "child_res.h"

Child *child_create(ChildArgs args) {
  Child *child;

  child = malloc(sizeof(Child));
  child->file_name = args.file_name;

  child->names = malloc(sizeof(ChildArgs));
  memcpy(child->names, &args, sizeof(ChildArgs));

  /* TODO shmem */
  WELL(args.sem_name_i_want);
  WELL(args.sem_name_thank_you);
  child->sem_thank_you = sem_open(args.sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  /* waiting for signal that the other semaphore was created (by the parent) */
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return NULL;
  }
  WELL("waiting for parent to create his semaphore");
  sem_wait(child->sem_thank_you);
  child->sem_i_want = sem_open(args.sem_name_i_want, O_WRONLY, 0666, 0);
  if (child->sem_i_want == NULL) {
    perror("child's 'I want' semaphore");
    return NULL;
  }
  WELL("created child's own semaphore");

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  if (child->file_name)
    free(child->file_name);

  if (child->names)
    free(child->names);

  sem_unlink(child->names->sem_name_thank_you);
  sem_close(child->sem_i_want);
  sem_close(child->sem_thank_you);

  free(child);
}

int child_loop(Child *child) {
  ChildRes *res;
  int j;

  for (j = 0; j < 3; j++) {

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
    res->line_contents = "hello there";
    child_res_to_file(res, child->file_name);
    WELL("responce put in file");
    child_res_free(res);

    WELL("faking a time wasteful process");
    usleep(200000);
  }

  WELL("loop done");
  return 0;
}

