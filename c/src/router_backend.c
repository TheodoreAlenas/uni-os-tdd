#include <stdlib.h>
#include <string.h>

#include "router_backend.h"

RouterBackend *router_backend_create() {
  return malloc(sizeof(RouterBackend));
}

void router_backend_free(RouterBackend *rb) {
  if (rb == NULL)
    return;

  free(rb);
}

void router_backend_write_req(RouterBackend *rb, char *msg) {
  strcpy((char *) rb->buf, msg);
}

void router_backend_read_req(RouterBackend *rb, char **msg) {
  strcpy(*msg, (char *) rb->buf);
}

sem_t get_semaphore_of_child_with_pid(pid_t pid) {
  return *((sem_t *) malloc(sizeof(sem_t)));
}
