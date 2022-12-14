#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "dev_mode.h"
#include "req.h"

int chop(char *dest, char *str, int start, char end_char);

void req_send_done(char *shm) {
  shm[0] = '$';
  shm[1] = '\0';
}

int req_says_done(char *msg) {
  return msg[0] == '$';
}

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

int find_line(char *str, int line) {
  int i, j = 0;

  for (i = 0; i < line; i++) {

    while (str[j] != '\0' && str[j] != '\n')
      j++;

    if (str[j] == '\0')
      return -1;

    j++;
  }
  return j;
}

int isolate_one(char *dest, char *str, int size) {
  int i;

  for (i = 0; i < size; i++) {

    if (str[i] == '\n' || str[i] == '\0') {
      dest[i] = '\0';
      return 0;
    }

    dest[i] = str[i];
  }

  return -1;
}

char *isolate_test(char *dest, char *str, int line, int size) {
  int i;

  i = find_line(str, line);
  if (i < 0)
    return NULL;

  i = isolate_one(dest, str + i, size);
  if (i < 0)
    return NULL;

  return dest;
}

char *isolate_line(char *dest, char *str, int line) {
  return isolate_test(dest, str, line, MAX_LINE_LEN);
}
