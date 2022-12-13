#ifndef REQ_H
#define REQ_H

typedef struct {
  unsigned child;
  int segment;
  int error;
} Req;

Req *req_parse(char *msg, Req *result);

#endif
