#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "loop.h"
#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "../both/req.h"
#include "res.h"

typedef struct { int file_segment; int line_in_segment; } SegmAndLine;
typedef struct {
  struct timespec req_time;
  struct timespec res_start;
  struct timespec res_end;
} ThreeTimespecs;
void post_and_wait(const Child *child, ThreeTimespecs * t);
SegmAndLine write_a_request(const Child *child, int *prev_line);
int little_offset(const Child *child, int prev_line);
void print_isolate_line_error(const Child *child, SegmAndLine d);
void tell_you_got_the_message(const Child *child);
void record_and_wait(const Child *child,
    ThreeTimespecs *t, SegmAndLine d, char *content);



/* for README, do-a-cycle */
void do_a_cycle(const Child *child, int *prev_line) {
  char isolated_line[MAX_LINE_LEN];
  int err, i;
  SegmAndLine req;
  ThreeTimespecs time_data;

  req = write_a_request(child, prev_line);
  post_and_wait(child, &time_data);
  err = !isolate_line(isolated_line,
      child->shmem_thank_you, req.line_in_segment);
  tell_you_got_the_message(child);

  if (err)
    print_isolate_line_error(child, req);
  else
    record_and_wait(child, &time_data, req, isolated_line);
}
/* end of snippet */



int child_loop_backend(const Child *child) {
  int i, prev_line;

  WELLL(printf("%s", child->names->file_name));
  srand(getpid());
  prev_line = rand() % child->lines_in_file;

  for (i = 0; i < child->names->loops; i++)
    do_a_cycle(child, &prev_line);

  WELL("loop done");
  return 0;
}

int little_offset(const Child *child, int prev_line) {
  int len, random, shuffled, normalized;

  len = child->names->file_segment_length;
  random = rand() % len;
  shuffled = prev_line + random - len / 2;
  normalized = shuffled % child->lines_in_file;

  return normalized;
}

SegmAndLine write_a_request(const Child *child, int *prev_line) {
  char req_str[MAX_REQUEST_LEN];
  int i, line_in_file, random;
  SegmAndLine d;

  if (rand() % 8 == 0)
    line_in_file = rand() % child->lines_in_file;
  else
    line_in_file = little_offset(child, *prev_line);

  d.file_segment = line_in_file / child->names->file_segment_length;
  d.line_in_segment = line_in_file % child->names->file_segment_length;


  sprintf(req_str, "<%d,%d>", d.file_segment, d.line_in_segment);
  /* for README, back-to-front-writing */
  for (i = MAX_REQUEST_LEN - 1; i >= 0; i--)
    child->shmem_i_want[i] = req_str[i];
  /* end of snippet */

  return d;
}

void print_isolate_line_error(const Child *child, SegmAndLine d) {
  fprintf(stderr,
      "child %d couldn't find "
      "line %d in segment %d ('%c%c...')\n",
      child->names->id, d.line_in_segment, d.file_segment,
      child->shmem_thank_you[0],
      child->shmem_thank_you[1]);
}

void record(const Child *child,
    ThreeTimespecs *t, SegmAndLine d, char *content) {
  ChildRes res;

  res.file_segment = d.file_segment;
  res.line_in_segment = d.line_in_segment;
  res.application_time_in_ns = t->req_time.tv_nsec;
  res.responce_time_in_ns = t->res_end.tv_nsec - t->res_start.tv_nsec;
  strcpy(res.line_contents, content);

  child_res_to_file(&res, child->names->file_name);
}

void post_and_wait(const Child *child, ThreeTimespecs * t) {

  clock_gettime(CLOCK_MONOTONIC, &t->res_start);
  t->req_time.tv_nsec = t->res_start.tv_nsec - t->res_end.tv_nsec;

  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);

  clock_gettime(CLOCK_MONOTONIC, &t->res_end);
}

void tell_you_got_the_message(const Child *child) {
  WELL("sending 'got the message'");
  req_send_done(child->shmem_i_want);
  sem_post(child->sem_i_want);
}

void record_and_wait(const Child *child,
    ThreeTimespecs *t, SegmAndLine d, char *content) {
  record(child, t, d, content);
  usleep(child->names->microsecond_delay);
  sem_wait(child->sem_thank_you);
}

