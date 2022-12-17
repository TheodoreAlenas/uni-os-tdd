#ifndef SHMEM_H
#define SHMEM_H

void *shmem_create_i_want(const char *name, unsigned long max_lines);
void *shmem_create_thank_you(const char *name, unsigned long max_lines);
void *shmem_open_i_want(const char *name, unsigned long max_lines);
void *shmem_open_thank_you(const char *name, unsigned long max_lines);
void shmem_free(const char *name);

#endif
