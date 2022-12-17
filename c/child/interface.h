#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>
#include <unistd.h>
#include "struct.h"

Child *child_create(const ChildArgs *args);
void child_free(Child *child);
void child_loop(const Child *child);

#endif
