#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "req.h"

int chop(char *dest, char *str, int start, char end_char);
void req_parse_dry(char *msg, Req *result);

Req *req_parse(char *msg, Req *result) {
  req_parse_dry(msg, result);
  return result;
}

void req_parse_dry(char *msg, Req *result) {
  int comma_pos, gt_pos, length, i;
  char segment[MAX_REQUEST_LEN], line[MAX_REQUEST_LEN];

  if (msg[0] != '<') {
    result->error = 1;
    return;
  }

  comma_pos = chop(segment, msg, 1, ',');
  if (comma_pos < 0) {
    result->error = 1;
    return;
  }

  gt_pos = chop(line, msg, comma_pos + 1, '>');
  if (gt_pos < 0) {
    result->error = 1;
    return;
  }

  result->segment = atoi(segment);
  result->error = 0;
}

int chop(char *dest, char *str, int start, char end_char) {
  int i;

  if (str[start] == end_char)
    return -1;

  for (i = start; i < MAX_REQUEST_LEN; i++) {

    if (str[i] == end_char) {
      *dest = '\0';
      return i;
    }

    if (str[i] <= '9' && str[i] >= '0')
      *(dest++) = str[i];

    else
      return -1;
  }
  return -1;
}
