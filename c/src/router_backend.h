#ifndef ROUTER_BACKEND_H
#define ROUTER_BACKEND_H

typedef struct {

} RouterBackend;

RouterBackend *router_backend_create(unsigned num_of_children, unsigned long request_size);
void router_backend_free(RouterBackend *rb);

#endif
