#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"

typedef struct {
  unsigned child;
  int segment;
} Req;

int testable_wait(const Parent *r);
int testable_post(const Parent *r, unsigned child);
int testable_sprintf(void *shm, char *str1, char *str2);
Req testable_parse_req(const void *shm);
char *testable_read_file_segment(const Parent *parent, unsigned long segment);

int loops(Parent *r, int children, int per_child);
int one_cycle(Parent *r);

typedef enum { MSG_ERROR, MSG_SEGMENT_REQUEST, MSG_YOU_CAN_SWITCH_SEGMENTS } MsgType;
typedef struct { MsgType type; int child; int segment; } Msg;
typedef struct { void *shm; int head; int step; int size; } MsgCycler;
void notification(Parent *r);
Msg check_one_msg(MsgCycler *msg_cycler);
void clean_msg(Msg msg);
int store(Parent *r, int segment);
int count_down_for_changing_segment(Parent *r);
int one_cycle_idea(Parent *r, MsgCycler *msg_cycler);


#endif
