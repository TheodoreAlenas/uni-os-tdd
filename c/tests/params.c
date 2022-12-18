#include "test.h"
#include "../before/raw_params_exposed.h"

static ParamPos pp;
static int dollar_is_right;

void cb1(char *params, int argc, char **argv) {
  param_pos_init(&pp, params);
}

void cb2(char *params, int argc, char **argv) {
  int i;

  while (params[i] != '$')
    i++;

  dollar_is_right = (i % pp.len == 0);
}

void test_params() {

  raw_params_callback(NULL, 0, NULL, cb1);
  announce("param_pos_init",
      0 <= pp.short_flag &&
      pp.short_flag < pp.long_flag &&
      pp.long_flag < pp.help &&
      pp.help < pp.value_buffer &&
      pp.value_buffer < pp.end &&
      pp.end < 164
      );

  raw_params_callback(NULL, 0, NULL, cb2);
  announce("param_dollar_is_right", dollar_is_right);
}
