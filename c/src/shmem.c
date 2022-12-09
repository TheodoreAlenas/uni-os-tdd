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

void *shmem_create(const char *name, unsigned long max_lines, int oflag, int prot) {
  const unsigned long SIZE = MAX_LINE_LEN * max_lines;
  int shm_fd;
  void* ptr;
  shm_fd = shm_open(name, oflag, 0666);

  /* changing the size of the shared memory segment */
  ftruncate(shm_fd, SIZE);
  ptr = mmap(0, SIZE, prot, MAP_SHARED, shm_fd, 0);

  return ptr;
}

void *shmem_create_read_only(const char *name, unsigned long max_lines) {
  return shmem_create(name, max_lines, O_CREAT | O_RDWR, PROT_READ);
}
void *shmem_create_write_only(const char *name, unsigned long max_lines) {
  return shmem_create(name, max_lines, O_CREAT | O_RDWR, PROT_WRITE);
}
void *shmem_open_read_only(const char *name, unsigned long max_lines) {
  return shmem_create(name, max_lines, O_RDWR, PROT_READ);
}
void *shmem_open_write_only(const char *name, unsigned long max_lines) {
  return shmem_create(name, max_lines, O_RDWR, PROT_WRITE);
}

void shmem_free(const char *name) {
  WELL("");
  shm_unlink(name);
}

void shmem_test_fill(void *shmem) {
  char* message_0 = "Hello";
  char* message_1 = "World!";

  sprintf(shmem, "%s", message_0);

  shmem += strlen(message_0);
  sprintf(shmem, "%s", message_1);
  shmem += strlen(message_1);
}
