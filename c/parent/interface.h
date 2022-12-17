#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>

#include "struct.h"


Parent *parent_create(const ParentParams *pp, char **sem_names);
void parent_free(Parent *r);
int parent_loop(Parent *r);
int parent_waitpid(const Parent *r);

#endif
