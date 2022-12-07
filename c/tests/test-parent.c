#include "../src/fork.h"

#include "test.h"

void test_semaphores() {
  char *s;

  s = get_semaphore_name(10);
  announce("get_semaphore_name_says_10_for_10",
      strcmp(s, "sem10") == 0);
}
