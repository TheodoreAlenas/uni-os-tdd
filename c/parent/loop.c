#include <semaphore.h>
#include <string.h>
#include <stdio.h>

#include "../both/constants.h"
#include "../both/dev_mode.h"
#include "../both/req.h"
#include "loop.h"
#include "loop_exposed.h"
#include "stack.h"

int copy_and_clear_req(MsgCycler *msg_cycler, char *req_str);

void handle_same_segment(LoopState *s);
void handle_other_segment(ParentLoopParams *r, int child, int new_segment);

void handle_req_saying_got_it(LoopState *s);
void handle_req_saying_i_want(LoopState *s, char *req_str);

int he_asked_alone(LoopState *s, int new_segment);
int should_pop_requests(int readers, Stack *requests);
void pop_requests(LoopState *s);
void swap_segment(LoopState *s, int new_segment);


void single_loop(LoopState *s, MsgCycler *msg_cycler) {
  char req_str[MAX_REQUEST_LEN];

  WELLL(printf("waiting for notification. %d readers on current, %d.",
        s->readers, s->current_segment));
/* for README, parent-loop */
  _sem_wait(s->r->sem_yes_please);

  s->child = copy_and_clear_req(msg_cycler, req_str);

  if (req_says_got_it(req_str))
    handle_req_saying_got_it(s);
  else
    handle_req_saying_i_want(s, req_str);
/* end of snippet */
}


#ifndef TEST

#include "file_segment.h"
int _read_file_segment(Parent *parent, void *shm, int segment) {
  return testable_read_file_segment(parent, shm, segment);
}
int _sem_wait(sem_t *s) {
  return sem_wait(s);
}
int _sem_post(sem_t **sems, int child) {
  return sem_post(sems[child]);
}

#endif


int parent_loop(Parent *r) {
  ParentLoopParams p;
  LoopState s;
  MsgCycler msg_cycler;
  Stack requests;
  int j, total_notifications;

  stack_init(&requests, r->num_of_children);

  p.sem_yes_please = r->sem_yes_please;
  p.sems_youre_ready = r->sems_youre_ready;
  p.shmem_youre_ready = r->shmem_youre_ready;
  p.requests = &requests;

  s.r = &p;
  s.readers = 0;
  s.current_segment = -1;
  s.child = 0;
  s.parent = r;

  msg_cycler.head = 0;
  msg_cycler.messages = r->shmem_yes_please;
  msg_cycler.size = r->num_of_children;

  total_notifications = 2 * r->num_of_children * r->loops_per_child;

  for (j = 0; j < total_notifications; j++) {
    single_loop(&s, &msg_cycler);
    if (j % 1024 == 0)
      printf("progress: %d/%d\n", j, total_notifications);
  }
  printf("progress: %d/%d\n", j, total_notifications);

  return 0;
}

void handle_same_segment(LoopState *s) {
  (s->readers)++;
  WELLL(printf("telling child #%d that its file segment is ready", s->child));
  _sem_post(s->r->sems_youre_ready, s->child);
}

void handle_other_segment(ParentLoopParams *r, int child, int new_segment) {
  Item *req_item;
  WELL("");
  req_item = malloc(sizeof(Item));
  req_item->child = child;
  req_item->file_segment = new_segment;

  stack_push(r->requests, req_item);

  WELLL(stack_print_inline(r->requests));
}

void handle_req_saying_got_it(LoopState * s) {
  WELL("");
  s->readers--;
  _sem_post(s->r->sems_youre_ready, s->child);

  if (should_pop_requests(s->readers, s->r->requests))
    pop_requests(s);
}

int should_pop_requests(int readers, Stack *requests) {

  WELLL(printf("readers: %d, request stack emptiness: %d",
        readers, stack_is_empty(requests)));

  return readers == 0 && !stack_is_empty(requests);
}

struct for_each_args { sem_t **sems; int *readers; };

int update_readers_and_tell_child(Item *item, void *r) {
  struct for_each_args *a = r;
  (*(a->readers))++;
  return _sem_post(a->sems, item->child);
}

void pop_requests(LoopState *s) {
  int err;
  Item *top_item;
  struct for_each_args a;
  WELL("");

  top_item = s->r->requests->items[s->r->requests->size - 1];
  swap_segment(s, top_item->file_segment);

  a.readers = &(s->readers);
  a.sems = s->r->sems_youre_ready;
  /* for README, pop-requests */
  stack_for_all_of_segment(s->r->requests,
      update_readers_and_tell_child, &a);
  /* end of snippet */
  WELLL(stack_print_inline(s->r->requests));
}

void swap_segment(LoopState *s, int new_segment) {
  int err;

  err = _read_file_segment(s->parent, s->r->shmem_youre_ready, new_segment);
  s->current_segment = new_segment;
  WELLL(printf("as %d, saved '%c...'", new_segment, s->r->shmem_youre_ready[0]));
}

void handle_req_saying_i_want(LoopState *s, char *req_str) {
  int err, i;
  int new_segment = -1;

  WELL("");
  new_segment = req_parse(req_str);

  if (new_segment < 0) {
    fprintf(stderr, "invalid request by child #%d ('%c%c...', ASCII %d %d)\n",
        s->child, req_str[0], req_str[1], req_str[0], req_str[1]);
    new_segment = 0;
  }

  if (he_asked_alone(s, new_segment))
    swap_segment(s, new_segment);
  /* the current_segment might be mutated */

  if (new_segment == s->current_segment)
    handle_same_segment(s);
  else
    handle_other_segment(s->r, s->child, new_segment);
}

int he_asked_alone(LoopState *s, int new_segment) {

  if (!stack_is_empty(s->r->requests))
    return 0;

  if (new_segment == s->current_segment)
    return 0;

  if (s->readers != 0)
    return 0;

  return 1;
}

int copy_and_clear_req(MsgCycler *msg_cycler, char *req_str) {
  char *req_ptr;

  req_ptr = msg_cycler_find(msg_cycler);
  WELLL(printf("cycler has req_ptr '%s' and child %d",
        req_ptr, msg_cycler->head));

  strcpy(req_str, req_ptr);
  *req_ptr = '\0';

  return msg_cycler->head;
}
