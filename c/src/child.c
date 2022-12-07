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
  child->sem_i_want = sem_open(args.sem_name_i_want, O_CREAT | O_WRONLY, 0666, 0);
  child->sem_thank_you = sem_open(args.sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_i_want == NULL || child->sem_thank_you == NULL) {
    perror("child's semaphore");
    return NULL;
  }


  WELL("about to post the semaphore");
  WELL(child->names->sem_name_i_want);
  sem_post(child->sem_i_want);
  WELL("posted");
  WELL("about to wait the semaphore");
  sem_wait(child->sem_thank_you);
  WELL("somehow it's done");

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

  free(child);
}

int child_loop(Child *child) {
  ChildRes *res;

  WELL("");
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

  return 0;
}

