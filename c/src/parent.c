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

sem_t *init_sem_and_broadcast(const Parent *r);

Parent *parent_create(const ParentParams *pp) {
  int i;
  Parent *r = malloc(sizeof(Parent));

  WELL("");
  r->pp = pp;
  r->requests = stack_create(r->pp->num_of_children);
  WELLL(printf("%s, %s", r->pp->shmem_name_yes_please, r->pp->shmem_name_youre_ready));
  r->shmem_yes_please = shmem_create_write_only(r->pp->shmem_name_yes_please, r->pp->num_of_children);
  r->shmem_youre_ready = shmem_create_write_only(r->pp->shmem_name_youre_ready, r->pp->file_segment_length);
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
  for (i = 0; i < r->pp->num_of_children; i++) {
    testable_post(r, i);
  }

  return s;
}

int parent_loop(Parent *r) {
  int child = 0, j;
  MsgCycler msg_cycler;
  char req[MAX_LINE_LEN], *req_ptr;

  msg_cycler.head = 0;
  msg_cycler.messages = r->shmem_yes_please;
  msg_cycler.size = r->pp->num_of_children;

  for (j = 0; j < r->pp->num_of_children * r->pp->loops_per_child; j++) {
    WELL("waiting for notification");
    testable_wait(r);
    //sem_wait(r->sem_yes_please);

    req_ptr = msg_cycler_find(&msg_cycler);
    child = msg_cycler.head;
    WELLL(printf("cycler has req_ptr %p and child %d", req_ptr, child));

    strcpy(req, req_ptr);
    *req_ptr = '\0';

    char *segment = testable_read_file_segment(r, 1);
    /* printf("%s\n", segment); */
    /* TODO shmem */
    free(segment);
    testable_sprintf(r->shmem_youre_ready, "okay then! Take %s", req);
    WELLL(printf("saved '%s'", r->shmem_youre_ready));

    WELLL(printf("telling child #%d that its file segment is ready", child));
    testable_post(r, child);
  }
  WELL("loop done");

  return 0;
}

int loops(Parent *r, int children, int per_child) {
  int i;
  for (i = 0; i < children * per_child; i++)
    one_cycle(r);
  return 0;
}

int one_cycle(Parent *r) {
  Req req;
  char *s;
  s = malloc(64);

  testable_wait(r);
  testable_post(r, req.child);
  /* usleep */
  req = testable_parse_req(NULL);
  testable_read_file_segment(r, 0);
  testable_sprintf(s, "hep%s", " bro");
  testable_post(r, req.child);
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
