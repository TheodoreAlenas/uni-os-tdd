#ifndef REQ_H
#define REQ_H

void req_send_got_it(char *shm);
int req_says_got_it(char *msg);
int req_parse(const char *msg);
char *isolate_line(char *dest, char *str, int line);
char *isolate_test(char *dest, char *str, int line, int size);

#endif
