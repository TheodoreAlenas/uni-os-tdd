#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

#include "interface.h"
#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "loop.h"
#include "../both/shmem.h"

int try_opening_sem_i_want(Child *child, const ChildArgs *args);

int child_init(Child *child, const ChildArgs *args) {

  child->names = args;
  WELLL(printf("file name: %s", child->names->file_name));

  child->sem_thank_you = sem_open(args->sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return -1;
  }

  WELL("waiting for the parent to create his semaphore");
  sem_wait(child->sem_thank_you);

  /* for README, child-shmem-offset */
  child->shmem_i_want = shmem_open_i_want(
      args->shmem_name_i_want,
      args->num_of_children) + args->id * MAX_REQUEST_LEN;
  /* end of snippet */
  child->shmem_thank_you = shmem_open_thank_you(
      args->shmem_name_thank_you,
      args->file_segment_length);

  return try_opening_sem_i_want(child, args);
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

int try_opening_sem_i_want(Child *child, const ChildArgs *args) {

  child->sem_i_want = sem_open(args->sem_name_i_want, O_WRONLY, 0666, 0);
  if (child->sem_i_want == NULL) {
    perror("child's 'I want' semaphore");
    sem_unlink(args->sem_name_thank_you);
    sem_close(child->sem_thank_you);
    return -1;
  }
  WELL("created child's own semaphore");
  return 0;
}

int child_loop(const Child *child) {
  return child_loop_backend(child);  /* sorry */
}

