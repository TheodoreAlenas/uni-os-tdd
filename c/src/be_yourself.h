#ifndef BE_YOURSELF_H
#define BE_YOURSELF_H

#include "params.h"
#include "child_data.h"

int be_parent(Params *p, ChildData *children);
int be_child(Params *p, unsigned child_index, char *sem_name);


#endif
