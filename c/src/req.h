#ifndef REQ_H
#define REQ_H

typedef struct {
  unsigned child;
  int segment;
} Req;

int req_parse(char *msg, Req *result);

#endif
