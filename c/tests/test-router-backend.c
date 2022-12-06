#include "../src/router_backend.h"
#include "test.h"


#define REQUEST_SIZE 256

void test_router_backend() {

  RouterBackend *rb;
  int num_of_children = 3, i;
  char *msg;
  sem_t sem, sem2;
  void *buf;

  msg = malloc(REQUEST_SIZE);

  rb = router_backend_create(2, REQUEST_SIZE);
  announce("router_backend_fresh_isnt_initialized",
      !router_backend_is_initialized(rb));

  buf = malloc(1024);
  router_backend_map_buffer(rb, buf);
  announce("router_backend_map_buffer_maps", rb->buf == buf);
  announce("router_backend_with_buf_mapped_isnt_initialized",
      !router_backend_is_initialized(rb));

  router_backend_map_another_pid(rb, getpid());
  announce("router_backend_with_one_child_isnt_initialized",
      !router_backend_is_initialized(rb));

  router_backend_map_another_pid(rb, getpid() + 1);
  announce("router_backend_is_initialized",
      router_backend_is_initialized(rb));

  router_backend_write_req(rb, "hello world");
  router_backend_read_req(rb, &msg);
  announce("router_backend_read_what_was_written",
      strcmp(msg, "hello world") == 0);

  sem = get_semaphore_of_child_with_pid(1);
  announce("router_backend_non_zero_sem", sizeof(sem) != 0);

  announce("router_backend_free_null_doesnt_crash", true);
  router_backend_free(NULL);
  announce("router_backend_free_legit_doesnt_crash", true);
  router_backend_free(rb);
}
#undef REQUEST_SIZE
