#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parent.h"
#include "file_segment.h"
#include "dev_mode.h"
#include "constants.h"
#include "parent_params.h"
#include "shmem.h"
#include "msg_cycler.h"
#include "req.h"

sem_t *init_sem_and_broadcast(const Parent *r);

Parent *parent_create(const ParentParams *pp) {
  int i;
  Parent *r = malloc(sizeof(Parent));

  WELL("");
  r->pp = pp;
  r->requests = stack_create(r->pp->num_of_children);
  WELLL(printf("%s, %s", r->pp->shmem_name_yes_please, r->pp->shmem_name_youre_ready));
  r->shmem_yes_please = shmem_create_i_want(r->pp->shmem_name_yes_please, r->pp->num_of_children);
  r->shmem_youre_ready = shmem_create_thank_you(r->pp->shmem_name_youre_ready, r->pp->file_segment_length);
  r->sem_yes_please = init_sem_and_broadcast(r);
  if (r->sem_yes_please == NULL)
    return NULL;

  return r;
}

void parent_free(Parent *r) {
  WELL("(not freeing ParentParams)");
  stack_free(r->requests);
  if (r->sem_yes_please) {
    sem_unlink(r->pp->sem_name_yes_please);
    sem_close(r->sem_yes_please);
  }
  shmem_free(r->pp->shmem_name_yes_please);
  shmem_free(r->pp->shmem_name_youre_ready);

  free(r);
}

sem_t *init_sem_and_broadcast(const Parent *r) {
  sem_t *s;
  unsigned i;

  s = sem_open(r->pp->sem_name_yes_please, O_CREAT | O_WRONLY, 0666, 0);
  if (s == NULL) {
    perror("parent's 'yes please' semaphore");
    return NULL;
  }

  WELL("signaling that the semaphore is ready");
  for (i = 0; i < r->pp->num_of_children; i++)
    sem_post(r->pp->children[i].semaphore);

  return s;
}

typedef enum { MSG_ERROR, MSG_SEGMENT_REQUEST, MSG_YOU_CAN_SWITCH_SEGMENTS } MsgType;
typedef struct { MsgType type; int child; int segment; } Msg;
void notification(Parent *r);
void clean_msg(Msg msg);
int store(Parent *r, int segment);
int count_down_for_changing_segment(Parent *r);

int parent_loop(Parent *r) {
  int child = 0, j, current_segment = -1, new_segment = -1, err;
  MsgCycler msg_cycler;
  char req_str[MAX_REQUEST_LEN], *req_ptr;

  msg_cycler.head = 0;
  msg_cycler.messages = r->shmem_yes_please;
  msg_cycler.size = r->pp->num_of_children;

  for (j = 0; j < r->pp->num_of_children * r->pp->loops_per_child; j++) {
    WELL("waiting for notification");
    sem_wait(r->sem_yes_please);

    req_ptr = msg_cycler_find(&msg_cycler);
    child = msg_cycler.head;
    WELLL(printf("cycler has req_ptr %p and child %d", req_ptr, child));

    strcpy(req_str, req_ptr);

    new_segment = req_parse(req_ptr);
    *req_ptr = '\0';

    err = testable_read_file_segment(r, r->shmem_youre_ready, new_segment);
    /* printf("%s\n", segment); */
    /* TODO shmem */
    WELLL(printf("saved '%c%c...'", ((char *) r->shmem_youre_ready)[0], ((char *) r->shmem_youre_ready)[1]));

    WELLL(printf("telling child #%d that its file segment is ready", child));
    sem_post(r->pp->children[child].semaphore);
  }
  WELL("loop done");

  return 0;
}

int parent_waitpid(const Parent *r) {
  int i, status;

  for (i = 0; i < r->pp->num_of_children; i++) {
    WELLL(printf("waiting pid %d", r->pp->children[i].pid));
    waitpid(r->pp->children[i].pid, &status, 0);
  }

  if (WEXITSTATUS(status))
    return 0;
  return -1;
}
