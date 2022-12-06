#include <stdlib.h>
#include <string.h>

#include "router_backend.h"

RouterBackend *router_backend_create(
    unsigned num_of_children,
    unsigned long request_size) {

  RouterBackend *rb;
  rb = malloc(sizeof(RouterBackend));

  rb->buf = NULL;

  rb->num_of_children = num_of_children;
  rb->mapped_children = malloc(sizeof(RouterKey) * rb->num_of_children);
  rb->remaining_pids_to_map = rb->num_of_children;

  rb->request_size = request_size;

  return rb;
}

void router_backend_free(RouterBackend *rb) {
  if (rb == NULL)
    return;

  if (rb->mapped_children)
    free(rb->mapped_children);

  free(rb);
}

void router_backend_write_req(RouterBackend *rb, char *msg) {
  strcpy((char *) rb->buf, msg);
}

int router_backend_map_another_pid(RouterBackend *rb, RouterKey pid) {
  -- rb->remaining_pids_to_map;
  return 0;
}
void router_backend_map_buffer(RouterBackend *rb, void *buf) {
  rb->buf = buf;
}

int router_backend_is_initialized(RouterBackend *rb) {
  return rb->buf != NULL && rb->remaining_pids_to_map == 0;
}

void router_backend_read_req(RouterBackend *rb, char **msg) {
  strcpy(*msg, (char *) rb->buf);
}

void router_backend_set_semaphore(RouterBackend *rb, RouterKey pid, RouterValue *sem) {
  memcpy(rb->buf, &sem, sizeof(RouterValue));
}
RouterValue *get_semaphore_of_child_with_pid(RouterKey pid) {
  return (RouterValue *) malloc(sizeof(RouterValue));
}
