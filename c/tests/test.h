#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../src/constants.h"

void announce(char *function_name, bool passed);
int get_total_tests();
int get_total_tests_passed();
int get_total_tests_failed();
void test_stack();
void test_arg_parse();
void test_get_names();
void test_fork();
void test_parent();
void test_msg_cycler();

#endif
