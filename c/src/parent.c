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
  r->shmem_yes_please = shmem_create_read_only(r->pp->shmem_name_yes_please, 1);
  r->shmem_youre_ready = shmem_create_write_only(r->pp->shmem_name_youre_ready, 1);
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
    testable_post(r->pp->children[i].semaphore);
  }

  return s;
}

int parent_loop(Parent *r) {
  sem_t *send_me, *wait_me;
  int i, j;

  char *req;
  req = malloc(MAX_LINE_LEN);
  if (req == NULL) {
    perror("parent's malloc for req");
    return -1;
  }

#ifndef TEST
  for (j = 0; j < 2; j++) {
    for (i = 0; i < r->pp->num_of_children; i++) {
      WELL("waiting for anyone to ask something");
      testable_wait(r->sem_yes_please);
      testable_post(r->pp->children[i].semaphore);
      usleep(10000);

      WELLL(printf("request says '%s'", r->shmem_yes_please));

      char *segment = parent_read_file_segment(r, 1);
      /* printf("%s\n", segment); */
      /* TODO shmem */
      free(segment);
      testable_sprintf(r->shmem_youre_ready, "okay then! Take %s", (char *) r->shmem_yes_please);

      WELLL(printf("telling child #%d that its file segment is ready", i));
      testable_post(r->pp->children[i].semaphore);
    }
  }
  WELL("loop done");
#else

#endif

  return 0;
}

int loops(int children, int per_child) {
  int i;
  for (i = 0; i < children * per_child; i++)
    one_cycle();
  return 0;
}

int one_cycle() {
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
