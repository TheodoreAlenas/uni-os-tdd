#ifndef BE_YOURSELF_H
#define BE_YOURSELF_H

#include "params.h"

int testable_fork();
int be_parent(Params *p, char **sem_names);
int be_child(Params *p, unsigned child_index, char *sem_name);


#endif
