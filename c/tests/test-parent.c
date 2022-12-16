#include "test.h"
#include "../src/parent_loop_exposed.h"

static char *messages, *youre_ready;

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
  ParentLoopParams p;
  LoopState s;
  MsgCycler msg_cycler;
  Stack requests;
  int j, total_notifications;
  int num_of_children = 3, loops_per_child = 4;

  char messages_original[] =
    "<0,0>\0          "
    "<1,1>\0          "
    "<2,2>\0          "
    ;
  youre_ready = malloc(256);
  messages = malloc(3 * 16);
  for (j = 0; j < 3 * 16; j++)
    messages[j] = messages_original[j];

  stack_init(&requests, num_of_children);

  p.sem_yes_please = NULL;
  p.sems_youre_ready = NULL;
  p.shmem_youre_ready = youre_ready;
  p.requests = &requests;

  s.r = &p;
  s.readers = 0;
  s.current_segment = -1;
  s.child = 0;
  s.parent = NULL;

  msg_cycler.head = 0;
  msg_cycler.messages = messages;
  msg_cycler.size = num_of_children;

  total_notifications = 2 * num_of_children * loops_per_child;

  single_loop(&s, &msg_cycler);

  announce("hi from parent", 0 == _read_file_segment(NULL, NULL, 0));
}
