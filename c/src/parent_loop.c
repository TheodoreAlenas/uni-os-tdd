#include <semaphore.h>
#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "dev_mode.h"
#include "file_segment.h"
#include "req.h"
#include "parent_loop.h"
#include "stack.h"

typedef struct {
  Parent *r; int readers; int current_segment; int child;
} LoopState;

int copy_and_clear_req(MsgCycler *msg_cycler, char *req_str);

void handle_same_segment(LoopState * s);
void handle_other_segment(Parent *r, int child, int new_segment);

void handle_req_saying_read(LoopState * s);
void handle_req_saying_i_want(LoopState *s, char *req_str);

int he_asked_alone(LoopState *s, int new_segment);
int should_pop_requests(int readers, Stack *requests);
void pop_requests(LoopState * s);
void swap_segment(LoopState * s, int new_segment);

void single_loop(LoopState *s, MsgCycler *msg_cycler, char *req_str);


/* for README, parent-loop */
void single_loop(LoopState *s, MsgCycler *msg_cycler, char *req_str) {

  WELLL(printf("waiting for notification. %d readers on current.", s->readers));
  sem_wait(s->r->sem_yes_please);

  s->child = copy_and_clear_req(msg_cycler, req_str);

  if (req_says_done(req_str))
    handle_req_saying_read(s);
  else
    handle_req_saying_i_want(s, req_str);
}
/* end of snippet */

int parent_loop_backend(Parent *r) {
  LoopState s;
  MsgCycler msg_cycler;
  int j, total_notifications;
  char req_str[MAX_REQUEST_LEN];

  s.r = r;
  s.readers = 0;
  s.current_segment = -1;
  s.child = 0;

  msg_cycler.head = 0;
  msg_cycler.messages = r->shmem_yes_please;
  msg_cycler.size = r->pp->num_of_children;

  total_notifications = 2 * r->pp->num_of_children * r->pp->loops_per_child;

  for (j = 0; j < total_notifications; j++)
    single_loop(&s, &msg_cycler, req_str);

  WELL("loop done");

  return 0;
}

void handle_same_segment(LoopState *s) {
  (s->readers)++;
  WELLL(printf("telling child #%d that its file segment is ready", s->child));
  sem_post(s->r->sems_youre_ready[s->child]);
}

void handle_other_segment(Parent *r, int child, int new_segment) {
  Item *req_item;
  WELL("");
  req_item = malloc(sizeof(Item));
  req_item->child = child;
  req_item->file_segment = new_segment;
  stack_push(r->requests, req_item);
  WELLL(stack_print_inline(r->requests));
}

void handle_req_saying_read(LoopState * s) {
  WELL("");
  s->readers--;

  if (should_pop_requests(s->readers, s->r->requests))
    pop_requests(s);
}

int should_pop_requests(int readers, Stack *requests) {

  WELLL(printf("readers: %d, request stack emptiness: %d",
        readers, stack_is_empty(requests)));

  return readers == 0 && !stack_is_empty(requests);
}

struct for_each_args { sem_t **sems; int *readers; };

int tell_child(Item *item, void *r) {
  struct for_each_args *a;

  WELL("");
  a = (struct for_each_args *) r;
  (*(a->readers))++;
  return sem_post(a->sems[item->child]);
}

void pop_requests(LoopState * s) {
  int err;
  Item *top_item;
  struct for_each_args a;
  WELL("");

  top_item = s->r->requests->items[s->r->requests->size - 1];
  swap_segment(s, top_item->file_segment);

  a.readers = &(s->readers);
  a.sems = s->r->sems_youre_ready;
  stack_for_all_of_segment(s->r->requests, tell_child, &a);
}

void swap_segment(LoopState * s, int new_segment) {
  int err;

  /* TODO nope */
  err = testable_read_file_segment(s->r, s->r->shmem_youre_ready, new_segment);
  s->current_segment = new_segment;
  WELLL(printf("as %d, saved '%c...'", new_segment, ((char *) s->r->shmem_youre_ready)[0]));
}

void handle_req_saying_i_want(LoopState *s, char *req_str) {
  int err;
  int new_segment;

  WELL("");
  new_segment = req_parse(req_str);
  if (new_segment < 0) {
    fprintf(stderr, "invalid request by child #%d ('%c%c...')\n",
        s->child, req_str[0], req_str[1]);
    new_segment = 0;
  }

  if (he_asked_alone(s, new_segment))
    swap_segment(s, new_segment);
  /* the current_segment might be mutated */

  if (new_segment == s->current_segment)
    handle_same_segment(s);
  else
    handle_other_segment(s->r, s->child, new_segment);

  s->current_segment = new_segment;
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
