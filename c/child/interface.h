#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>
#include <unistd.h>
#include "struct.h"

int child_init(Child *child);
void child_free(Child *child);
int child_loop(const Child *child);

#endif
