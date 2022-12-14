#ifndef REQ_H
#define REQ_H

int req_parse(char *msg);
int isolate_line(char *dest, char *str, int line);

#endif
