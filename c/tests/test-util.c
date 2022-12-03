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

void test_one(bool (*f) (void), char *name) {
  bool passed;
  printf("%sTesting function '%s' ... %s", GREEN, name, RESET);
  passed = f();

  if (passed) {
    printf("%sPassed%s\n", GREEN, RESET);
  }
  else {
    printf("%sFailed%s\n", RED, RESET);
  }

}
