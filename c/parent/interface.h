#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>

#include "struct.h"


int parent_init(Parent *r, const ParentParams *pp, char **sem_names);
void parent_free(Parent *r);
int parent_loop(Parent *r);
int parent_waitpid(const Parent *r);

#endif
