#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

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

void *shmem_create_i_want(const char *name, unsigned long max_lines) {
  return shmem_backend(name, max_lines * MAX_REQUEST_LEN, O_CREAT | O_RDWR, PROT_WRITE);
}
void *shmem_create_thank_you(const char *name, unsigned long max_lines) {
  return shmem_backend(name, max_lines * MAX_LINE_LEN, O_CREAT | O_RDWR, PROT_WRITE);
}
void *shmem_open_i_want(const char *name, unsigned long max_lines) {
  return shmem_backend(name, max_lines * MAX_REQUEST_LEN, O_RDWR, PROT_WRITE);
}
void *shmem_open_thank_you(const char *name, unsigned long max_lines) {
  return shmem_backend(name, max_lines * MAX_LINE_LEN, O_RDWR, PROT_READ);
}

void shmem_free(const char *name) {
  shm_unlink(name);
}

