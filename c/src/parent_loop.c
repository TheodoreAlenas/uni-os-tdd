#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "dev_mode.h"
#include "file_segment.h"
#include "req.h"
#include "parent_loop.h"
#include "stack.h"


int parent_loop_backend(Parent *r) {
  int child = 0, j, current_segment = -1, new_segment = -1, readers = 0, total_notifications;
  MsgCycler msg_cycler;
  char req_str[MAX_REQUEST_LEN];

  msg_cycler.head = 0;
  msg_cycler.messages = r->shmem_yes_please;
  msg_cycler.size = r->pp->num_of_children;

  total_notifications = 2 * r->pp->num_of_children * r->pp->loops_per_child;

  /* for README, parent-loop */
  for (j = 0; j < total_notifications; j++) {
    WELLL(printf("waiting for notification. %d readers on current.", readers));
    sem_wait(r->sem_yes_please);

    copy_and_clear_req(&msg_cycler, &child, req_str);

    if (req_says_done(req_str))
      handle_done(r, &readers, child, &current_segment);
    else
      handle_not_done(r, req_str, &readers, &new_segment, &current_segment, child);

    current_segment = new_segment;
  }
  /* end of snippet */
  WELL("loop done");

  return 0;
}

void handle_same_segment(Parent *r, int *readers, int child) {
  (*readers)++;
  WELLL(printf("telling child #%d that its file segment is ready", child));
  sem_post(r->sems_youre_ready[child]);
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

void handle_done(Parent *r, int *readers, int child, int *current_segment) {
  WELL("");
  (*readers)--;

  if (should_pop_requests(*readers, r->requests))
    pop_requests(r, readers, current_segment, child);
}

int should_pop_requests(int readers, Stack *requests) {

  WELLL(printf("readers: %d, request stack emptiness: %d",
        readers, stack_is_empty(requests)));

  if (readers != 0)
    return 0;

  if (!stack_is_empty(requests))
    return 0;

  return 1;
}

void pop_requests(Parent *r, int *readers, int *current_segment, int child) {
  int err;
  Item **item, **top_item;

  /* TODO nope */
  swap_segment(r, readers, current_segment, (*top_item)->file_segment, child);
}

void swap_segment(Parent *r, int *readers, int *current_segment, int new_segment, int child) {
  int err;

  /* TODO nope */
  err = testable_read_file_segment(r, r->shmem_youre_ready, new_segment);
  *current_segment = new_segment;
  WELLL(printf("as %d, saved '%c...'", new_segment, ((char *) r->shmem_youre_ready)[0]));
}

void handle_not_done(Parent *r, char *req_str, int *readers, int *new_segment, int *current_segment, int child) {
  int err;

  WELL("");
  *new_segment = req_parse(req_str);
  if (*new_segment < 0) {
    fprintf(stderr, "invalid request by child #%d ('%c%c...')\n",
        child, req_str[0], req_str[1]);
    *new_segment = 0;
  }

  if (*new_segment == *current_segment)
    handle_same_segment(r, readers, child);
  else {
    if (stack_is_empty(r->requests))
      swap_segment(r, readers, current_segment, *new_segment, child);
    handle_other_segment(r, child, *new_segment);
  }

}

void copy_and_clear_req(MsgCycler *msg_cycler, int *child, char *req_str) {
  char *req_ptr;

  req_ptr = msg_cycler_find(msg_cycler);
  *child = msg_cycler->head;
  WELLL(printf("cycler has req_ptr '%s' and child %d", req_ptr, *child));

  strcpy(req_str, req_ptr);
  *req_ptr = '\0';

}
