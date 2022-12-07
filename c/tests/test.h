#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void announce(char *function_name, bool passed);
int get_total_tests();
int get_total_tests_passed();
int get_total_tests_failed();
void test_stack();
void test_arg_parse();
void test_semaphores();

#endif
