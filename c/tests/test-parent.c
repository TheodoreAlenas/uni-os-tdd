#include "../src/parent.h"
#include "../src/file_segment.h"
#include "test.h"

enum Action { A_READ_REQ, A_READ_FILE, A_WRITE, A_POST, A_WAIT };
static int head;
static enum Action actions[256];

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

Req testable_parse_req(void *shm) {
  Req req;
  actions[head++] = A_READ_REQ;
  return req;
}

char *parent_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res;
  actions[head++] = A_READ_FILE;
  res = malloc(64);
  strcpy(res, "contents of\nfile\nsegment");
  return res;
}

void test_parent() {

  announce("test_parent_exists_but_cant_do_much", true);
  announce("test_parent_loop_1_1_returns_zero", 0 == loops(1, 1));
  //parent_loop(NULL);
}

