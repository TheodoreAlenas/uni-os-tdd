#include "test.h"
#include "../src/msg_cycler.h"

void test_msg_cycler() {

  MsgCycler mc;
  mc.head = 0;
  mc.size = 1;
  mc.messages = malloc(MAX_LINE_LEN * 1);
  sprintf(mc.messages, "a");
  announce("finds_one", 0 == strcmp("a", msg_cycler_find(&mc)));

  mc.head = 0;
  mc.size = 1;
  sprintf(mc.messages, "");
  announce("finds_none", NULL == msg_cycler_find(&mc));
}

