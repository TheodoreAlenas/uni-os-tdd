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


void announce(char *function_name, bool passed) {
  printf("%sTesting function '%s' ... %s", GREEN, function_name, RESET);

  if (passed) {
    printf("%sPassed%s\n", GREEN, RESET);
  }
  else {
    printf("%sFailed%s\n", RED, RESET);
  }
}
