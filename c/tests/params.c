#include "test.h"
#include "../before/raw_params_exposed.h"

static ParamPos pp;

void cb1(char *params, int argc, char **argv) {
  param_pos_init(&pp, params);
  announce("param_pos_init",
      0 <= pp.short_flag &&
      pp.short_flag < pp.long_flag &&
      pp.long_flag < pp.help &&
      pp.help < pp.takes_value &&
      pp.takes_value < pp.value_buffer &&
      pp.value_buffer < pp.end &&
      pp.end < 164
      );
}

void cb2(char *params, int argc, char **argv) {
  int i = 0;

  while (params[i] != '$')
    i++;

  announce("param_dollar_is_right", i % pp.len == 0);
}

void cb3(char *params, int argc, char **argv) {
  int pos;

  pos = find_short_matching(params, "-c", &pp);
  announce("c_flag_exists", pos > 0);
  announce("c_flag_placed_right", 0 ==
      strcmp(params + pos + pp.short_flag, "c"));

  pos = find_long_matching(params, "--help", &pp);
  announce("help_flag_exists", pos > 0);
  announce("help_flag_placed_right", 0 ==
      strcmp(params + pos + pp.long_flag, "help"));
}

void cb4(char *params, int argc, char **argv) {
  raw_params_help(params, &pp);
}

void test_params() {

  raw_params_callback(NULL, 0, NULL, cb1);
  raw_params_callback(NULL, 0, NULL, cb2);
  raw_params_callback(NULL, 0, NULL, cb3);
  raw_params_callback(NULL, 0, NULL, cb4);
}
