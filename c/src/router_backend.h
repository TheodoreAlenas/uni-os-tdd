#ifndef ROUTER_BACKEND_H
#define ROUTER_BACKEND_H

#include <unistd.h>

#include "sys/types.h"
#include "semaphore.h"

#ifdef TEST
typedef int RouterKey;
typedef int RouterValue;
#else
typedef pid_t RouterKey;
typedef sem_t RouterValue;
#endif

typedef struct {
  void *buf;
  unsigned num_of_children;
  RouterKey *mapped_children;
  unsigned remaining_pids_to_map;
  unsigned long request_size;
} RouterBackend;

RouterBackend *router_backend_create(
    unsigned num_of_children,
    unsigned long request_size);
void router_backend_free(RouterBackend *rb);

int router_backend_map_another_pid(RouterBackend *rb, RouterKey pid);
void router_backend_map_buffer(RouterBackend *rb, void *buf);

int router_backend_is_initialized(RouterBackend *rb);

void router_backend_write_req(RouterBackend *rb, char *msg);
void router_backend_read_req(RouterBackend *rb, char **msg);

void router_backend_set_semaphore(RouterBackend *rb, RouterKey pid, RouterValue *sem);
RouterValue *get_semaphore_of_child_with_pid(RouterKey pid);

#endif
