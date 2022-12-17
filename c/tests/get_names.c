#include "../src/get_names.h"

#include "test.h"

void test_get_names() {
  char *s, *expected, *pid;

  expected = malloc(732);

  sprintf(expected, "sem10%u", getpid());

  s = get_semaphore_name(10);
  announce("get_semaphore_name_says_10_for_10",
      strcmp(s, expected));

  sprintf(expected, "../output_dir/index-123-pid-%u", getpid());

  s = get_output_file_name("../output", 123);
  announce("get_output_file_name_says_123_right",
      strcmp(s, expected));
}

