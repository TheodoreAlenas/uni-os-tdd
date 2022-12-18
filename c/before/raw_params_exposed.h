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
} ParamPos;

void param_pos_init(ParamPos *pp, char *p);
void fill_them(char *p, int argc, char **argv);
void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (char *params, int argc, char **argv));

#endif
