#include "test.h"
#include "../src/params.h"
/* for README, fork-in-test */
#include "../src/fork.h"
#include "../src/be_yourself.h"


static int num_of_forks;

int testable_fork() {
  num_of_forks++;
  return fork();
}

/* end of snippet */
int be_parent(Params *p, ChildData *children) {
  return 0;
}

int be_child(Params *p, unsigned child_index, char *sem_name) {
  return 0;
}

int one_test(char *argv[3], int num, char *msg) {
  pid_t start_pid;
  int success = 1;
  Params p;

  start_pid = getpid();
  num_of_forks = 0;

  handle_forks(parameters_parse(&p, 3, argv));

  /* this is meant to happen.
   * Any resources created before handle_forks
   * must be freed by all the children. */
  if (getpid() != start_pid)
    exit(0);

  if (num_of_forks != num)
    success = 0;

  announce(msg, success);

  return 1;
}

void test_fork() {
  char *path = "/some/path/to/binary";
  char *c_flag = "-c";
  char *num_0 = "0";
  char *num_1 = "1";
  char *num_2 = "2";
  char *num_3 = "3";
  char *num_64 = "64";
  char *argv_0[3] = { path, c_flag, num_0 };
  char *argv_1[3] = { path, c_flag, num_1 };
  char *argv_2[3] = { path, c_flag, num_2 };
  char *argv_3[3] = { path, c_flag, num_3 };
  char *argv_64[3] = { path, c_flag, num_64 };
  pid_t start_pid;

  start_pid = getpid();

  one_test(argv_0, 0, "test_fork_0_child");
  one_test(argv_1, 1, "test_fork_1_child");
  one_test(argv_2, 2, "test_fork_2_childen");
  one_test(argv_3, 3, "test_fork_3_childen");
  one_test(argv_64, 64, "test_fork_64_childen");
}

