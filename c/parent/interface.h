#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>
#include "struct.h"

#include "loop.h"

int parent_init(Parent *r);
void parent_free(Parent *r);
int parent_waitpid(const Parent *r);

#endif
