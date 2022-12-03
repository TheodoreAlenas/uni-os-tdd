#include <stdio.h>

#include "stack.h"


int main() {
  Stack *s = stack_create(5);
  printf("hello\n");
#ifdef DEV
  printf("dev is defined\n");
#endif
}
