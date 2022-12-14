#ifndef REQ_H
#define REQ_H

int req_parse(char *msg);
char *isolate_line(char *dest, char *str, int line);
char *isolate_test(char *dest, char *str, int line, int size);

#endif
