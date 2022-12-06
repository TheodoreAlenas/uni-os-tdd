#include "../src/router_backend.h"
#include "test.h"


#define REQUEST_SIZE 256

void test_router_backend() {

  RouterBackend *rb;
  int num_of_children = 3, i;
  char *msg;
  sem_t sem;

  msg = malloc(REQUEST_SIZE);

  rb = router_backend_create();
  rb->num_of_children = 3;
  rb->request_size = REQUEST_SIZE;
  rb->buf = malloc(1024);

  router_backend_write_req(rb, "hello world");
  router_backend_read_req(rb, &msg);
  announce("router_backend_read", strcmp(msg, "hello world") == 0);

  sem = get_semaphore_of_child_with_pid(1);
  announce("router_backend_non_zero_sem", sizeof(sem) != 0);

  announce("router_backend_free_null_doesnt_crash", true);
  router_backend_free(NULL);
  announce("router_backend_free_legit_doesnt_crash", true);
  router_backend_free(rb);
}
#undef REQUEST_SIZE
