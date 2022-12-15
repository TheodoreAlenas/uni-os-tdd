#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "parent.h"
#include "dev_mode.h"
#include "constants.h"
#include "parent_params.h"
#include "parent_loop.h"
#include "shmem.h"
#include "req.h"

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

  r->sems_youre_ready = open_child_created_sems(r, sem_names);
  r->sem_yes_please = init_sem_and_broadcast(r);
  if (r->sem_yes_please == NULL)
    return NULL;

  return r;
}

void parent_free(Parent *r) {
  int i;
  WELL("(not freeing ParentParams)");
  stack_free(r->requests);
  if (r->sem_yes_please) {
    sem_unlink(r->pp->sem_name_yes_please);
    sem_close(r->sem_yes_please);
  }
  for (i = 0; i < r->pp->num_of_children; i++) {
    if (r->sems_youre_ready[i])
      sem_close(r->sems_youre_ready[i]);
  }
  shmem_free(r->pp->shmem_name_yes_please);
  shmem_free(r->pp->shmem_name_youre_ready);

  free(r);
}

sem_t **open_child_created_sems(const Parent *r, char **sem_names) {
  int i;
  sem_t **to_return;
  char msg[128];

  to_return = malloc(r->pp->num_of_children * sizeof(sem_t*));

  for (i = 0; i < r->pp->num_of_children; i++) {
    /* the parent comes second, so the semaphore may be opened either way */
    to_return[i] = sem_open(sem_names[i], O_CREAT | O_RDONLY, 0666, 0);
    WELL(sem_names[i]);

    if (to_return[i] == NULL) {
      sprintf(msg, "parent trying to open child %d's '%s' created semaphore",
          i, sem_names[i]);
      perror(msg);
    }
  }

  return to_return;
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
    sem_post(r->sems_youre_ready[i]);

  return s;
}


int parent_loop(Parent *r) {
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

    copy_and_clear_req(&msg_cycler, child, req_str);

    if (req_says_done(req_str))
      handle_done(r, &readers, child);
    else
      handle_not_done(r, req_str, &readers, &new_segment, &current_segment, child);

    current_segment = new_segment;
  }
  /* end of snippet */
  WELL("loop done");

  return 0;
}

int parent_waitpid(const Parent *r) {
  int i, status;

  for (i = 0; i < r->pp->num_of_children; i++) {
    WELLL(printf("waiting child the %d-th time", i));
    waitpid(-1, &status, 0);
  }

  if (WEXITSTATUS(status))
    return 0;
  return -1;
}
