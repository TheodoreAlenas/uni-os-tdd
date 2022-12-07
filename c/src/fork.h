#ifndef FORK_H
#define FORK_H

#include "params.h"

int handle_forks(Params *p, void *shmem);
char *get_semaphore_name(unsigned child_index);
char *get_output_file_name(char *output_dir, unsigned child_index);

#endif
