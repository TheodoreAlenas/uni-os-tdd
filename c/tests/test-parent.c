#include "../src/parent.h"
#include "test.h"

static int num_of_wait;
static int num_of_post;

char *parent_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res = malloc(64);
  strcpy(res, "hi there\nhow are you\n");
  return res;
}

int testable_wait(sem_t *sem) {

}

int testable_post(sem_t *sem) {

}

void test_parent() {
  Parent *r;
  ParentParams *pp;
  announce("test_parent_exists", true);
  pp = parent_params_create();
  r = parent_create(NULL);
  parent_loop(r);
  parent_free(r);
}

