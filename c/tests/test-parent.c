#include "../src/parent.h"
#include "../src/file_segment.h"
#include "test.h"

enum Action { A_READ_REQ, A_READ_FILE, A_WRITE, A_POST, A_WAIT };
static int action_head, res_head;
static enum Action actions[256];
static unsigned res[256];

int testable_wait(const Parent *r) {
  actions[action_head++] = A_WAIT;
  return 0;
}

int testable_post(const Parent *r, unsigned child) {
  actions[action_head++] = A_POST;
  res[res_head++] = child;
  return 0;
}

int testable_sprintf(void *shm, char *str1, char *str2) {
  actions[action_head++] = A_WRITE;
  return 0;
}

Req testable_parse_req(const void *shm) {
  Req req;
  actions[action_head++] = A_READ_REQ;
  req.child = 3;
  req.segment = 3;
  return req;
}

char *testable_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res;
  actions[action_head++] = A_READ_FILE;
  res = malloc(64);
  strcpy(res, "contents of\nfile\nsegment");
  return res;
}

bool has(enum Action a) {
  int i;
  for (i = 0; i < action_head; i++)
    if (actions[i] == a)
      return true;
  return false;
}

void test_parent() {

  announce("test_parent_loop_1_2_returns_zero", 0 == loops(NULL, 1, 2));
  announce("test_parent_has_read_req", has(A_READ_REQ));
  announce("test_parent_has_read_file", has(A_READ_FILE));
  announce("test_parent_has_write", has(A_WRITE));
  announce("test_parent_has_post", has(A_POST));
  announce("test_parent_has_wait", has(A_WAIT));
  announce("test_parent_posts_to_the_right", res[0] == 3 && res[1] == 3);

  action_head = 0;
  //parent_loop(NULL);
}

