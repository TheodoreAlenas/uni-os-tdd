#include "test.h"
#include "../src/msg_cycler.h"

void test_msg_cycler() {

  MsgCycler mc;
  mc.head = 0;
  mc.size = 1;
  mc.messages = malloc(MAX_LINE_LEN * 10);
  sprintf(mc.messages, "a");
  announce("finds_one", 0 == strcmp("a", msg_cycler_find(&mc)));

  mc.head = 0;
  mc.size = 1;
  sprintf(mc.messages, "");
  announce("finds_none", NULL == msg_cycler_find(&mc));

  mc.head = 1;
  mc.size = 2;
  sprintf(mc.messages, "a");
  announce("loops_back", 0 == strcmp("a", msg_cycler_find(&mc)));

  mc.messages[MAX_LINE_LEN] = 'b';
  mc.messages[MAX_LINE_LEN + 1] = '\0';
  announce("continues", 0 == strcmp("b", msg_cycler_find(&mc)));

}

