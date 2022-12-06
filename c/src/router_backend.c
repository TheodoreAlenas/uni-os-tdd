#include "router_backend.h"
#include <stdlib.h>

RouterBackend *router_backend_create(
    unsigned num_of_children,
    unsigned long request_size) {

  RouterBackend *rb = NULL;
  rb = malloc(sizeof(RouterBackend));

  return rb;
}
