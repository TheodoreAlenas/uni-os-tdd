#ifndef MSG_CYCLER_H
#define MSG_CYCLER_H

typedef struct {
  char *messages;
  int head;
  int size;
} MsgCycler;

char *msg_cycler_find(MsgCycler *cycler);

#endif
