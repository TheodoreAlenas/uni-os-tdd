#include "test.h"

#ifdef NO_COLOR
#define RED     ""
#define GREEN   ""
#define RESET   ""
#else
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"
#endif

int TOTAL_TESTS_PASSED;
int TOTAL_TESTS;

int get_total_tests() { return TOTAL_TESTS; }
int get_total_tests_passed() { return TOTAL_TESTS_PASSED; }
int get_total_tests_failed() { return TOTAL_TESTS - TOTAL_TESTS_PASSED; }

void announce(char *function_name, bool passed) {
  TOTAL_TESTS++;
  printf("%sTest '%s' ... %s", GREEN, function_name, RESET);

  if (passed) {
    printf("%sPassed%s\n", GREEN, RESET);
    TOTAL_TESTS_PASSED++;
  }
  else {
    printf("%sFailed%s\n", RED, RESET);
  }
}
