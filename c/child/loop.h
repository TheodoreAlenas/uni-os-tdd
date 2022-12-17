#ifndef CHILD_LOOP_H
#define CHILD_LOOP_H

#include "struct.h"

void do_a_cycle(const Child *child, int *prev_line);
int child_loop_backend(const Child *child);

#endif
