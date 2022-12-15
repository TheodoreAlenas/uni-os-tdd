#ifndef PARENT_LOOP_H
#define PARENT_LOOP_H

#include "parent_struct.h"
#include "msg_cycler.h"

int parent_loop_backend(Parent *r);

void handle_same_segment(Parent *r, int *readers, int child);
void handle_other_segment(Parent *r, int child, int new_segment);
void handle_done(Parent *r, int *readers, int child);
int should_swap_segment(int readers, int new_segment, int current_segment);
void swap_segment(Parent *r, int *readers, int new_segment, int *current_segment, int child);
void handle_not_done(Parent *r, char *req_str, int *readers, int *new_segment, int *current_segment, int child);
void copy_and_clear_req(MsgCycler *msg_cycler, int *child, char *req_str);


#endif
