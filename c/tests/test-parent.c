#include "../src/parent.h"
#include "test.h"

enum Action { A_READ, A_WRITE, A_POST, A_WAIT };
static int head;
static enum Action actions[256];

char *parent_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res;
  res = malloc(64);
  strcpy(res, "hi there\nhow are you\n");
  return res;
}

int testable_wait(sem_t *sem) {
  actions[head++] = A_WAIT;
  return 0;
}

int testable_post(sem_t *sem) {
  actions[head++] = A_POST;
  return 0;
}

int testable_sprintf(void *shm, char *str1, char *str2) {
  actions[head++] = A_WRITE;
  return 0;
}

char *testable_shm_read(void *shm) {
  actions[head++] = A_READ;
  return 0;
}

void test_parent() {

  announce("test_parent_exists_but_cant_do_much", true);
  //parent_loop(NULL);
}

