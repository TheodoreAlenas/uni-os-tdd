#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "loop.h"
#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "../both/req.h"
#include "res.h"


int child_loop_backend(const Child *child) {
  int i;

  WELLL(printf("%s", child->names->file_name));

  for (i = 0; i < child->names->loops; i++)
    do_a_cycle(child);

  WELL("loop done");
  return 0;
}

typedef struct { int file_segment; int line_in_segment; } SegmAndLine;
typedef struct {
    struct timespec req_time;
    struct timespec res_start;
    struct timespec res_end;
} ThreeTimespecs;
void post_and_wait(const Child *child, ThreeTimespecs * t);


SegmAndLine write_a_request(const Child *child) {
  char req_str[MAX_REQUEST_LEN];
  int i;
  static int bullshit = 0;  /* TODO rename */
  SegmAndLine d;
  d.file_segment = (getpid() + !((bullshit++) % 4)) % 3;
  d.line_in_segment = child->names->id; //getpid() % 7;

  sprintf(req_str, "<%d,%d>", d.file_segment, d.line_in_segment);
  /* for README, back-to-front-writing */
  for (i = MAX_REQUEST_LEN - 1; i >= 0; i--)
    ((char *)child->shmem_i_want)[i] = req_str[i];
  /* end of snippet */

  return d;
}

void print_isolate_line_error(const Child *child, SegmAndLine d) {
  fprintf(stderr,
      "child %d couldn't find "
      "line %d in segment %d ('%c%c...')\n",
      child->names->id, d.line_in_segment, d.file_segment,
      ((char *) child->shmem_thank_you)[0],
      ((char *) child->shmem_thank_you)[1]);
}

void record(const Child *child, ThreeTimespecs *t, SegmAndLine d, char *content) {
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

void record_and_wait(const Child *child, ThreeTimespecs *t, SegmAndLine d, char *content) {
  record(child, t, d, content);
  usleep(child->names->microsecond_delay);
  sem_wait(child->sem_thank_you);
}

/* for README, do-a-cycle */
void do_a_cycle(const Child *child) {
  char isolated_line[MAX_LINE_LEN];
  int err, i;
  SegmAndLine req;
  ThreeTimespecs time_data;

  req = write_a_request(child);
  post_and_wait(child, &time_data);
  err = !isolate_line(
      isolated_line,
      child->shmem_thank_you,
      req.line_in_segment);
  tell_you_got_the_message(child);

  if (err)
    print_isolate_line_error(child, req);
  else
    record_and_wait(child, &time_data, req, isolated_line);
}
/* end of snippet */

