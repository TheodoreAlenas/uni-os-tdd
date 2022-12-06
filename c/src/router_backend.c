#include "router_backend.h"
#include <stdlib.h>

RouterBackend *router_backend_create() {
  return malloc(sizeof(RouterBackend));
}

void router_backend_free(RouterBackend *rb) {
  if (rb == NULL)
    return;

  free(rb);
}

void router_backend_write_req(RouterBackend *rb, char *msg) {

}

void router_backend_read_req(RouterBackend *rb, char **msg) {
}
