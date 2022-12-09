void *shmem_create(const char *name, unsigned long max_lines);
void shmem_free(const char *name, void *shmem);
void shmem_test_fill(void *shmem);

