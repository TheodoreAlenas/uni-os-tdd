#include "../src/parent.h"
#include "../src/file_segment.h"
#include "test.h"

enum Action { A_READ_REQ, A_READ_FILE, A_WRITE, A_POST, A_WAIT };
static int action_head, res_head;
static enum Action actions[256];
static unsigned res[256];

void notification(Parent *r) {}
Msg check_one_msg(MsgCycler *msg_cycler) { Msg msg; return msg; }
void say_you_cleaned_it(unsigned child) {}
void clean_msg(Msg msg) {}
int store(Parent *r, int segment) { return 0; }  /* doesn't necessarily add to the stack */
int count_down_for_changing_segment(Parent *r) { return 0; }
int segment_should_change(const Parent *r) { return 0; }
void change_segment(Parent *r) {}

void handle_message(Parent *r, Msg msg);

int one_cycle_idea(Parent *r, MsgCycler *msg_cycler) {
  Msg msg;

  notification(r);
  msg = check_one_msg(msg_cycler);
  clean_msg(msg);
  say_you_cleaned_it(msg.child);
  handle_message(r, msg);
  return 0;
}

/*
void mock_child() {

  const unsigned long SIZE_REQ = MAX_LINE_LEN * max_lines;
  const unsigned long SIZE_RES = MAX_LINE_LEN * max_lines;
  int shm_req_fd, shm_res_fd;
  void *shm_req, *shm_res;
  shm_req_fd = shm_open(name, oflag, 0666);
  shm_res_fd = shm_open(name, oflag, 0666);
  ftruncate(shm_req_fd, SIZE_REQ);
  ftruncate(shm_res_fd, SIZE_RES);
  shm_req = mmap(0, SIZE_REQ, prot, MAP_SHARED, shm_req_fd, 0);
  shm_req = mmap(0, SIZE_RES, prot, MAP_SHARED, shm_res_fd, 0);

  exit(0);
}
*/

void handle_message(Parent *r, Msg msg) {

  if (msg.type == MSG_SEGMENT_REQUEST)
    store(r, msg.segment);

  else if (msg.type == MSG_YOU_CAN_SWITCH_SEGMENTS)
    count_down_for_changing_segment(r);

  if (segment_should_change(r))
    change_segment(r);
}


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
/*
  announce("test_parent_loop_1_2_returns_zero", 0 == loops(NULL, 1, 2));
  announce("test_parent_has_read_req", has(A_READ_REQ));
  announce("test_parent_has_read_file", has(A_READ_FILE));
  announce("test_parent_has_write", has(A_WRITE));
  announce("test_parent_has_post", has(A_POST));
  announce("test_parent_has_wait", has(A_WAIT));
  announce("test_parent_posts_to_the_right", res[0] == 3 && res[1] == 3);
  announce("test_parent_1_wait", actions[0] == A_WAIT);
  announce("test_parent_2_post", actions[1] == A_POST);
  announce("test_parent_3_read_req", actions[2] == A_READ_REQ);*/

  announce("test_parent_one_cycle_idea", 0 == one_cycle_idea(NULL, NULL));
  announce("test_parent_", 0 == one_cycle_idea(NULL, NULL));

  action_head = 0;
  //parent_loop(NULL);
}

