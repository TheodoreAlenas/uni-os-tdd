#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "req.h"

int chop(char *dest, char *str, int start, char end_char);

int req_parse(char *msg) {
  int comma_pos, gt_pos, length, i;
  char segment[MAX_REQUEST_LEN], line[MAX_REQUEST_LEN];

  if (msg[0] != '<')
    return -1;

  comma_pos = chop(segment, msg, 1, ',');
  if (comma_pos < 0)
    return -1;

  gt_pos = chop(line, msg, comma_pos + 1, '>');
  if (gt_pos < 0)
    return -1;

  return atoi(segment);
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

/* untested TODO */
int isolate_line(char *dest, char *str, int line) {
  int i;

  for (i = 0; i < line; i++) {

    while (*str != '\0' && *str != '\n')
      str++;

    if (*str == '\0')
      return -1;

    str++;
  }

  while (*str != '\n' && *str != '\0') {
    *(dest++) = *(str++);
  }

  *dest = '\0';

  return 0;
}
