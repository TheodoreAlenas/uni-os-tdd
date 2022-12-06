#include "router_backend.h"
#include <stdlib.h>

RouterBackend *router_backend_create(
    unsigned num_of_children,
    unsigned long request_size) {

  RouterBackend *rb = NULL;
  rb = malloc(sizeof(RouterBackend));

  return rb;
}

void router_backend_free(RouterBackend *rb) {
  if (rb == NULL)
    return;

  free(rb);
}

void router_backend_write_req(RouterBackend *rb, char *msg) {

}
