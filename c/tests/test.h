#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdbool.h>

void announce(char *function_name, bool passed);
void test_stack();
void test_arg_parse();

#endif
