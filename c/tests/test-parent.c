#include "../src/parent.h"
#include "../src/file_segment.h"
#include "test.h"

enum Action { A_READ_REQ, A_READ_FILE, A_WRITE, A_POST, A_WAIT };
static int head;
static enum Action actions[256];

int testable_wait(const Parent *r) {
  actions[head++] = A_WAIT;
  return 0;
}

int testable_post(const Parent *r, unsigned child) {
  actions[head++] = A_POST;
  return 0;
}

int testable_sprintf(void *shm, char *str1, char *str2) {
  actions[head++] = A_WRITE;
  return 0;
}

Req testable_parse_req(const void *shm) {
  Req req;
  actions[head++] = A_READ_REQ;
  return req;
}

char *testable_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res;
  actions[head++] = A_READ_FILE;
  res = malloc(64);
  strcpy(res, "contents of\nfile\nsegment");
  return res;
}

bool has(enum Action a) {
  int i;
  for (i = 0; i < head; i++)
    if (actions[i] == a)
      return true;
  return false;
}

void test_parent() {

  announce("test_parent_exists_but_cant_do_much", true);
  announce("test_parent_loop_1_1_returns_zero", 0 == loops(NULL, 1, 1));
  announce("test_parent_has_read_req", has(A_READ_REQ));
  announce("test_parent_has_read_file", has(A_READ_FILE));
  announce("test_parent_has_write", has(A_WRITE));
  announce("test_parent_has_post", has(A_POST));
  announce("test_parent_has_wait", has(A_WAIT));

  head = 0;
  //parent_loop(NULL);
}

