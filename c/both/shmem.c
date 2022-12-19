#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "shmem.h"
#include "dev_mode.h"
#include "constants.h"

void *shmem_backend(const char *name, const unsigned long size, int oflag, int prot) {
  int shm_fd;
  void* ptr;
  WELLL(printf("name: %s, size: %d", name, size));
  shm_fd = shm_open(name, oflag, 0666);

  /* changing the size of the shared memory segment */
  ftruncate(shm_fd, size);
  ptr = mmap(0, size, prot, MAP_SHARED, shm_fd, 0);

  return ptr;
}

char *shmem_create_i_want(const Parent *parent) {
  return shmem_backend(
      parent->shmem_name_yes_please,
      parent->num_of_children * MAX_REQUEST_LEN,
      O_CREAT | O_RDWR, PROT_WRITE);
}
char *shmem_create_thank_you(const Parent *parent) {
  return shmem_backend(
      parent->shmem_name_youre_ready,
      parent->file_segment_length * MAX_LINE_LEN,
      O_CREAT | O_RDWR, PROT_WRITE);
}

char *shmem_open_i_want_with_offset(const Child *child) {
  char *shm;
  int offset;

  shm = shmem_backend(
      child->shmem_name_i_want,
      child->num_of_children * MAX_REQUEST_LEN,
      O_RDWR, PROT_WRITE);

  /* for README, child-shmem-offset */
  offset = child->id * MAX_REQUEST_LEN;
  return shm + offset;
  /* end of snippet */
}
char *shmem_open_thank_you(const Child *child) {
  return shmem_backend(
      child->shmem_name_thank_you,
      child->file_segment_length * MAX_LINE_LEN,
      O_RDWR, PROT_READ);
}

