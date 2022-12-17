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

void record(char *content, const Child *child, SegmAndLine d, int time1, int time2) {
  ChildRes res;

  res.file_segment = d.file_segment;
  res.line_in_segment = d.line_in_segment;
  res.application_time_in_ns = time1;
  res.responce_time_in_ns = time2;
  strcpy(res.line_contents, content);

  child_res_to_file(&res, child->names->file_name);

}

/* for README, do-a-cycle */
void do_a_cycle(const Child *child) {
  char isolated_line[MAX_LINE_LEN];
  int err, i;
  SegmAndLine d;
  struct timespec req_start, req_end, res_start, res_end;

  d = write_a_request(child);

  clock_gettime(CLOCK_MONOTONIC, &res_start);
  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);
  clock_gettime(CLOCK_MONOTONIC, &res_end);

  err = !isolate_line(isolated_line, child->shmem_thank_you, d.line_in_segment);

  WELL("sending 'got the message'");
  req_send_done(child->shmem_i_want);
  sem_post(child->sem_i_want);

  if (err) {
    print_isolate_line_error(child, d);
    return;
  }
  record(isolated_line, child, d, 3, res_end.tv_nsec - res_start.tv_nsec);
  usleep(child->names->microsecond_delay);

  sem_wait(child->sem_thank_you);
}
/* end of snippet */

