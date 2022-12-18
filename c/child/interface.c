#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

#include "interface.h"
#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "loop.h"
#include "../both/shmem.h"

int try_opening_sem_i_want(Child *child);

int child_init(Child *child) {
  int err, offset;
  char *shm;

  child->sem_thank_you = sem_open(child->sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return -1;
  }

  WELL("waiting for the parent to create his semaphore");
  sem_wait(child->sem_thank_you);
  WELL("able to read the lines");

  shm = shmem_open_i_want(
      child->shmem_name_i_want,
      child->num_of_children);

  /* for README, child-shmem-offset */
  offset = child->id * MAX_REQUEST_LEN;
  child->shmem_i_want = shm + offset;
  /* end of snippet */

  child->shmem_thank_you = shmem_open_thank_you(
      child->shmem_name_thank_you,
      child->file_segment_length);

  err = try_opening_sem_i_want(child);
  if (err)
    return err;

  child->lines_in_file = *((int *) child->shmem_thank_you);

  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);
  WELLL(printf("read that the file has %d lines", child->lines_in_file));

  return 0;
}

void child_free(Child *child) {

  sem_unlink(child->sem_name_thank_you);
  sem_close(child->sem_i_want);
  sem_close(child->sem_thank_you);
  shmem_free(child->shmem_name_i_want);
  shmem_free(child->shmem_name_thank_you);
}

int try_opening_sem_i_want(Child *child) {

  child->sem_i_want = sem_open(child->sem_name_i_want, O_WRONLY, 0666, 0);
  if (child->sem_i_want == NULL) {
    perror("child's 'I want' semaphore");
    sem_unlink(child->sem_name_thank_you);
    sem_close(child->sem_thank_you);
    return -1;
  }
  WELL("created child's own semaphore");
  return 0;
}

int child_loop(const Child *child) {
  return child_loop_backend(child);  /* sorry */
}

