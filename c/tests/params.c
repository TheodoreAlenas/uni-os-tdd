#include "test.h"
#include "../before/raw_params_exposed.h"

static ParamPos pp;

void cb(char *params, int argc, char **argv) {
  param_pos_init(&pp, params);
}

void test_params() {

  raw_params_callback(NULL, 0, NULL, cb);
  announce("param_pos_init",
      0 <= pp.short_flag &&
      pp.short_flag < pp.long_flag &&
      pp.long_flag < pp.help &&
      pp.help < pp.value_buffer &&
      pp.value_buffer < pp.end &&
      pp.end < 10000
      );
}
