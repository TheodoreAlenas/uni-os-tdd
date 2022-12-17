#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#include "child.h"
#include "constants.h"
#include "dev_mode.h"
#include "child_res.h"
#include "shmem.h"
#include "req.h"

Child *try_opening_sem_i_want(Child *child, const ChildArgs *args);
void do_a_cycle(const Child *c);

Child *child_create(const ChildArgs *args) {
  Child *child;

  child = malloc(sizeof(Child));

  child->names = args;
  WELLL(printf("file name: %s", child->names->file_name));

  child->sem_thank_you = sem_open(args->sem_name_thank_you, O_CREAT | O_RDONLY, 0666, 0);
  if (child->sem_thank_you == NULL) {
    perror("child's thank you semaphore");
    return NULL;
  }

  /* for README, child-shmem-offset */
  WELL("waiting for the parent to create his semaphore");
  sem_wait(child->sem_thank_you);

  child->shmem_i_want = shmem_open_i_want(args->shmem_name_i_want, args->num_of_children) + args->id * MAX_REQUEST_LEN;
  child->shmem_thank_you = shmem_open_thank_you(args->shmem_name_thank_you, args->file_segment_length);
  /* end of snippet */

  child = try_opening_sem_i_want(child, args);

  return child;
}

void child_free(Child *child) {
  if (child == NULL)
    return;

  sem_unlink(child->names->sem_name_thank_you);
  sem_close(child->sem_i_want);
  sem_close(child->sem_thank_you);
  shmem_free(child->names->shmem_name_i_want);
  shmem_free(child->names->shmem_name_thank_you);

  free(child);
}

Child *try_opening_sem_i_want(Child *child, const ChildArgs *args) {

  child->sem_i_want = sem_open(args->sem_name_i_want, O_WRONLY, 0666, 0);
  if (child->sem_i_want == NULL) {
    perror("child's 'I want' semaphore");
    sem_unlink(args->sem_name_thank_you);
    sem_close(child->sem_thank_you);
    return NULL;
  }
  WELL("created child's own semaphore");
  return child;
}

void child_loop(const Child *child) {
  int i;

  WELLL(printf("%s", child->names->file_name));

  for (i = 0; i < child->names->loops; i++)
    do_a_cycle(child);

  WELL("loop done");
}

typedef struct { int file_segment; int line_in_segment; } SegmAndLine;

SegmAndLine write_a_request(const Child *child) {
  char req_str[MAX_REQUEST_LEN];
  int i;
  static int bullshit = 0;  /* TODO rename */
  SegmAndLine d;
  d.file_segment = (getpid() + !((bullshit++) % 4)) % 3;
  d.line_in_segment = child->names->id; //getpid() % 7;

  /* for README, back-to-front-writing */
  sprintf(req_str, "<%d,%d>", d.file_segment, d.line_in_segment);
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
  char content[MAX_LINE_LEN];
  int err, i;
  SegmAndLine d;

  d = write_a_request(child);
  sem_post(child->sem_i_want);
  sem_wait(child->sem_thank_you);

  err = !isolate_line(content, child->shmem_thank_you, d.line_in_segment);

  WELL("sending 'got the message'");
  req_send_done(child->shmem_i_want);
  sem_post(child->sem_i_want);

  if (err) {
    print_isolate_line_error(child, d);
    return;
  }
  record(content, child, d, 3, 4);
  usleep(child->names->microsecond_delay);

  sem_wait(child->sem_thank_you);
}
/* end of snippet */

