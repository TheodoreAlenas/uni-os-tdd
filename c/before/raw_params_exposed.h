#ifndef RAW_PARAMS_EXPOSED_H
#define RAW_PARAMS_EXPOSED_H

#include "../both/constants.h"
#include "params.h"

typedef enum { P_STR, P_INT } RawPType;

typedef struct {

  char short_flag[4];
  char long_flag[64];
  char help[MAX_PARAM_LEN];

  RawPType type;

  int min_len;
  int max_len;

  char value_str[256];
  long value_int;

} RawParams;

typedef enum {
  WP_HELP, WP_PRINT,
  WP_CHILDREN, WP_LOOPS, WP_DELAY,
  WP_INPUT, WP_OUTPUT,
  WP_LINES,
  WP_I_WANT, WP_THANK_YOU,

  WP_PARAM_NUM
} WhichParam;

typedef struct {
  int short_flag;
  int long_flag;
  int help;
  int value_buffer;
  int end;
  int takes_value;
  int len;
} ParamPos;

void param_pos_init(ParamPos *pp, char *p);
void fill_them(char *p, int argc, char **argv);
void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (char *params, int argc, char **argv));
int find_short_matching(char *p, char *flag, ParamPos *pp);
int find_long_matching(char *p, char *flag, ParamPos *pp);

int get_is_long_flag(char *s);
int get_is_short_flag(char *s);
void set_long_flag(char *p, char *s, ParamPos *pp);
void set_short_flag(char *p, int argc, char **argv, int *i, ParamPos *pp);
void set_input_file(char *p, char *name, ParamPos *pp);
void set_output_dir(char *p, char *name, ParamPos *pp);

void raw_params_help(char *p, ParamPos *pp);

#endif
