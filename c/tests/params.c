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
  int i;
  char
    a1[] = "path/to/file",
    a2[] = "-c",
    a3[] = "2",
    a4[] = "--loops=4",
    a5[] = "--help",
    a6[] = "in";

  int c = 6;
  char *v[] = {a1, a2, a3, a4, a5, a6};

  fill_them(params, c, v);

  i = find_short_matching(params, "-c", &pp);
  announce("c_flag_passed", 0 == strcmp("2", params + i + pp.value_buffer));

  i = find_short_matching(params, "-r", &pp);
  announce("loops_flag_passed", 0 == strcmp("4", params + i + pp.value_buffer));

  i = find_short_matching(params, "-h", &pp);
  announce("help_flag_passed", 0 == strcmp("on", params + i + pp.takes_value));

  i = find_short_matching(params, "-i", &pp);
  announce("input_file_passed", 0 == strcmp("in", params + i + pp.value_buffer));
}

void test_params() {
  raw_params_callback(NULL, 0, NULL, cb1);
  raw_params_callback(NULL, 0, NULL, cb2);
  raw_params_callback(NULL, 0, NULL, cb3);
  raw_params_callback(NULL, 0, NULL, cb4);
}
