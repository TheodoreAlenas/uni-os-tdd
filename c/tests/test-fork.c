#include "test.h"
#include "../src/params.h"
#include "../src/fork.h"
#include "../src/be_yourself.h"


static int num_of_forks;

int testable_fork() {
  num_of_forks++;
  return fork();
}

int be_parent(Params *p, ChildData *children) {
  return 0;
}

int be_child(Params *p, unsigned child_index, char *sem_name) {
  return 0;
}

int one_test(char *argv[3], int num) {
  pid_t start_pid;

  start_pid = getpid();
  num_of_forks = 0;
  handle_forks(parameters_parse(3, argv));
  if (getpid() != start_pid)
    exit(0);

  if (num_of_forks != num)
    return 0;

  return 1;
}

void test_fork() {
  char *path = "/some/path/to/binary";
  char *c_flag = "-c";
  char *num_1 = "1";
  char *num_2 = "2";
  char *num_3 = "3";
  char *num_64 = "64";
  char *argv_1[3] = { path, c_flag, num_1 };
  char *argv_2[3] = { path, c_flag, num_2 };
  char *argv_3[3] = { path, c_flag, num_3 };
  char *argv_64[3] = { path, c_flag, num_64 };

  announce("test_fork_1_child", one_test(argv_1, 1));
  announce("test_fork_2_childen", one_test(argv_2, 2));
  announce("test_fork_3_childen", one_test(argv_3, 3));
  announce("test_fork_64_childen", one_test(argv_64, 64));
}

