#include <stdio.h>
#include <string.h>

#include "tests/test.h"

void call_all();
void call_individual_with_same_name(char *name);
void say_final_results();


int main(int argc, char **argv) {
  int i;

  if (argc == 1)
    call_all();

  for (i = 1; i < argc; i++)
    call_individual_with_same_name(argv[i]);

  say_final_results();
  return get_total_tests_failed();
}

void call_all() {
  test_stack();
  test_get_names();
  test_fork();
  test_msg_cycler();
  test_req();
  test_parent();
  test_params();
}

#define CONSIDER(n) else if (strcmp(name, #n) == 0) test_##n()

void call_individual_with_same_name(char *name) {
  if (0) return;
  CONSIDER(stack);
  CONSIDER(get_names);
  CONSIDER(fork);
  CONSIDER(msg_cycler);
  CONSIDER(req);
  CONSIDER(parent);
  CONSIDER(params);
}
#undef CONSIDER

void say_final_results() {
  printf("%d/%d passed, %d failed\n",
      get_total_tests_passed(),
      get_total_tests(),
      get_total_tests_failed());
}
