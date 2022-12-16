#include "test.h"
#include "../src/parent_loop.h"


int _read_file_segment(Parent *parent, void *shm, int segment) {
  return 0;
}
int _sem_wait(sem_t *s) {
  return 0;
}
int _sem_post(sem_t **sems, int child) {
  return 0;
}

void test_parent() {
  announce("hi from parent", 0 == _read_file_segment(NULL, NULL, 0));
}
