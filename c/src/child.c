#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

#include "child.h"
#include "constants.h"
#include "dev_mode.h"
#include "child_loop.h"
#include "shmem.h"

Child *try_opening_sem_i_want(Child *child, const ChildArgs *args);

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

  /* for README, child-shmem-offset */
  WELL("waiting for the parent to create his semaphore");
  sem_wait(child->sem_thank_you);

  child->shmem_i_want = shmem_open_i_want(args->shmem_name_i_want, args->num_of_children) + args->id * MAX_REQUEST_LEN;
  child->shmem_thank_you = shmem_open_thank_you(args->shmem_name_thank_you, args->file_segment_length);
  /* end of snippet */

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
  child_loop_backend(child);  /* sorry */
}

