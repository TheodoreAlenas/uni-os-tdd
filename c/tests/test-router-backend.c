#include "../src/router_backend.h"
#include "test.h"

void test_router_backend() {

  RouterBackend *rb;
  int num_of_children = 3;
  int request_size = 4;

  rb = router_backend_create(num_of_children, request_size);
  announce("router_backend_create", rb);

  announce("router_backend_free_null_doesnt_crash", true);
  router_backend_free(NULL);
  announce("router_backend_free_legit_doesnt_crash", true);
  router_backend_free(rb);
}
