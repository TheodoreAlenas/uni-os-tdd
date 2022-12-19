#ifndef SHMEM_H
#define SHMEM_H

#include "../parent/struct.h"
#include "../child/struct.h"

char *shmem_create_i_want(const Parent *parent);
char *shmem_create_thank_you(const Parent *parent);
char *shmem_open_i_want_with_offset(const Child *child);
char *shmem_open_thank_you(const Child *child);

#endif
