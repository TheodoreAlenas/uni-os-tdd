#include <stdio.h>
#include "dev_mode.h"
#include "msg_cycler.h"
#include "constants.h"

    /*
char *msg_cycler_find(MsgCycler *cycler) {
  int i;
  char *msg_spot;
  for (i = 0; i < cycler->size; i++) {

    WELLL(printf("loop %d/%d", cycler->head, cycler->size));
    cycler->head++;
    if (cycler->head >= cycler->size) {
      cycler->head = 0;
    WELL("full circle");
    }

    msg_spot = cycler->messages + cycler->head * MAX_LINE_LEN;
    if ('\0' != *msg_spot)
      return msg_spot;

    return cycler->messages;
  }
  return NULL;
}
      */

char *msg_cycler_find(MsgCycler *cycler) {
  return cycler->messages;
}
