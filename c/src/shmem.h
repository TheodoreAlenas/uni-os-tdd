void *shmem_create_read_only(const char *name, unsigned long max_lines);
void *shmem_create_write_only(const char *name, unsigned long max_lines);
void *shmem_open_read_only(const char *name, unsigned long max_lines);
void *shmem_open_write_only(const char *name, unsigned long max_lines);
void shmem_free(const char *name, void *shmem);
void shmem_test_fill(void *shmem);

