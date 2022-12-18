#include "test.h"
#include "../before/raw_params_exposed.h"

void cb1(ParamPos *p, Params *params, int argc, char **argv) {
  announce("param_pos_init",
      0 <= p->short_flag &&
      p->short_flag < p->long_flag &&
      p->long_flag < p->help &&
      p->help < p->takes_value &&
      p->takes_value < p->value_buffer &&
      p->value_buffer < p->end &&
      p->end < 164
      );
}

void cb2(ParamPos *p, Params *params, int argc, char **argv) {
  int i = 0;

  while (p->p[i] != '$')
    i++;

  announce("param_dollar_is_right", i % p->len == 0);
}

void cb3(ParamPos *p, Params *params, int argc, char **argv) {
  int pos;

  pos = find_short_matching(p, "-c");
  announce("c_flag_exists", pos > 0);
  announce("c_flag_placed_right", 0 ==
      strcmp(p->p + pos + p->short_flag, "c"));

  pos = find_long_matching(p, "--help");
  announce("help_flag_exists", pos > 0);
  announce("help_flag_placed_right", 0 ==
      strcmp(p->p + pos + p->long_flag, "help"));
}

void cb4(ParamPos *p, Params *params, int argc, char **argv) {
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

  fill_them(p, c, v);

  i = find_short_matching(p, "-c");
  announce("c_flag_passed", 0 == strcmp("2", p->p + i + p->value_buffer));

  i = find_short_matching(p, "-r");
  announce("loops_flag_passed", 0 == strcmp("4", p->p + i + p->value_buffer));

  i = find_short_matching(p, "-h");
  announce("help_flag_passed", 0 == strcmp("on", p->p + i + p->takes_value));

  i = find_short_matching(p, "-i");
  announce("input_file_passed", 0 == strcmp("in", p->p + i + p->value_buffer));
}

void test_params() {
  raw_params_callback(NULL, 0, NULL, cb1);
  raw_params_callback(NULL, 0, NULL, cb2);
  raw_params_callback(NULL, 0, NULL, cb3);
  raw_params_callback(NULL, 0, NULL, cb4);
}
