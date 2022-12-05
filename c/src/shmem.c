#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SHMEM_SIZE_PER_LINE 1024

/* TODO personalize the piece of code */
void *shmem_create(unsigned long max_lines) {
  unsigned long SIZE = SHMEM_SIZE_PER_LINE * max_lines;
  char* name = "OS";
  int shm_fd;
  void* ptr;
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  /* changing the size of the shared memory segment */
  ftruncate(shm_fd, SIZE);
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  return ptr;
}

void shmem_test_fill(void *shmem) {
  char* message_0 = "Hello";
  char* message_1 = "World!";

  sprintf(shmem, "%s", message_0);

  shmem += strlen(message_0);
  sprintf(shmem, "%s", message_1);
  shmem += strlen(message_1);
}
