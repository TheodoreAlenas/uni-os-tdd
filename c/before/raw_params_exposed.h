#ifndef RAW_PARAMS_EXPOSED_H
#define RAW_PARAMS_EXPOSED_H

#include "../both/constants.h"
#include "params.h"

typedef enum { P_STR, P_INT } RawPType;

typedef struct {
  char *p;
  int short_flag;
  int long_flag;
  int help;
  int value_type;
  int takes_value;
  int value_buffer;
  int end;
  int len;
} ParamPos;

void param_pos_init(ParamPos *p);
void fill_them(ParamPos *p, int argc, char **argv);
void fill_and_translate(ParamPos *p, Params *params, int argc, char **argv);
void translate(ParamPos *p, Params *params);
void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (ParamPos *s, Params *params, int argc, char **argv));
int find_short_matching(ParamPos *p, char *flag);
int find_long_matching(ParamPos *p, char *flag);

int get_is_long_flag(char *s);
int get_is_short_flag(char *s);
void set_long_flag(ParamPos *p, char *s);
void set_short_flag(ParamPos *p, int argc, char **argv, int *i);
void set_input_file(ParamPos *p, char *name);
void set_output_dir(ParamPos *p, char *name);

void raw_params_help(ParamPos *p);
void raw_params_print(ParamPos *p);

#endif
