#include <stdio.h>
#include "dev_mode.h"
#include "msg_cycler.h"
#include "constants.h"

#define CYCLE_HEAD if (++cycler->head >= cycler->size) cycler->head = 0

char *msg_cycler_find(MsgCycler *cycler) {
  int i;
  char *msg_spot;

  CYCLE_HEAD;

  for (i = 0; i < cycler->size; i++) {

    msg_spot = cycler->messages + cycler->head * MAX_REQUEST_LEN;
    if ('\0' != *msg_spot)
      return msg_spot;

    CYCLE_HEAD;
  }
  return NULL;
}

#undef CYCLE_HEAD
