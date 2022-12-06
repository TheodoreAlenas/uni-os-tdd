#ifndef ROUTER_BACKEND_H
#define ROUTER_BACKEND_H

typedef struct {
  void *shmem;
  unsigned num_of_children;
  unsigned long request_size;
} RouterBackend;

RouterBackend *router_backend_create();
void router_backend_free(RouterBackend *rb);
void router_backend_write_req(RouterBackend *rb, char *msg);
void router_backend_read_req(RouterBackend *rb, char **msg);

#endif
