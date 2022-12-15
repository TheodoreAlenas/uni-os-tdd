#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "src/parent.h"
#include "src/dev_mode.h"
#include "src/constants.h"
#include "src/parent_params.h"
#include "src/parent_loop.h"
#include "src/shmem.h"
#include "src/req.h"
sem_t *init_sem_and_broadcast(const Parent *r);
sem_t **open_child_created_sems(const Parent *r, char **sem_names);


Parent *parent_create(const ParentParams *pp, char **sem_names) {
  int i;
  Parent *r = malloc(sizeof(Parent));

  r->pp = pp;
  r->requests = stack_create(r->pp->num_of_children);

  WELLL(printf("%s, %s", r->pp->shmem_name_yes_please, r->pp->shmem_name_youre_ready));
  r->shmem_yes_please = shmem_create_i_want(r->pp->shmem_name_yes_please, r->pp->num_of_children);
  r->shmem_youre_ready = shmem_create_thank_you(r->pp->shmem_name_youre_ready, r->pp->file_segment_length);

  /* for README, blooper-fork-debug */
  r->sems_youre_ready = open_child_created_sems(r, sem_names);
#include <unistd.h>
  if (fork() == 0) {
    int i;
    for (i = 0; i < 50; i++) {

    WELLL(printf(" ------------------------------------------ | %5s | %5s | %5s | %5s | --------",
          r->shmem_yes_please + 0 * MAX_REQUEST_LEN,
          r->shmem_yes_please + 1 * MAX_REQUEST_LEN,
          r->shmem_yes_please + 2 * MAX_REQUEST_LEN,
          r->shmem_yes_please + 3 * MAX_REQUEST_LEN
          ));

    }
    exit(0);
  }

  r->sem_yes_please = init_sem_and_broadcast(r);
  /* end of snippet */
  if (r->sem_yes_please == NULL)
    return NULL;

  return r;
}
