#include "../src/parent.h"
#include "test.h"

char *parent_read_file_segment(const Parent *parent, unsigned long segment) {
  char *res = malloc(64);
  strcpy(res, "hi there\nhow are you\n");
  return res;
}

void test_parent() {
  announce("test_parent_exists", true);
}

