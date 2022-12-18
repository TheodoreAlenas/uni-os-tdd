#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "interface.h"
#include "../both/dev_mode.h"
#include "../both/constants.h"
#include "params.h"
#include "loop.h"
#include "../both/shmem.h"
#include "../both/req.h"

sem_t *init_sem_and_broadcast(const Parent *r);
sem_t **open_child_created_sems(const Parent *r, char **sem_names);
int count_lines_in_file(char *file_name);

int parent_init(Parent *r, const ParentParams *pp, char **sem_names) {
  int i;

  r->pp = pp;
  r->requests = stack_create(r->pp->num_of_children);

  WELLL(printf("%s, %s", r->pp->shmem_name_yes_please, r->pp->shmem_name_youre_ready));
  r->shmem_yes_please = shmem_create_i_want(r->pp->shmem_name_yes_please, r->pp->num_of_children);
  r->shmem_youre_ready = shmem_create_thank_you(r->pp->shmem_name_youre_ready, r->pp->file_segment_length);

  r->lines_in_file = count_lines_in_file(r->pp->file_name);
  if (r->lines_in_file == -1)
    return -1;
  *( (int *) r->shmem_youre_ready ) = r->lines_in_file;

  r->sems_youre_ready = open_child_created_sems(r, sem_names);
  r->sem_yes_please = init_sem_and_broadcast(r);
  if (r->sem_yes_please == NULL)
    return -1;

  /* waiting the children to read the number of lines */
  for (i = 0; i < r->pp->num_of_children; i++) {
    WELLL(printf("child %d also knows now", i));
    sem_wait(r->sem_yes_please);
    sem_post(r->sems_youre_ready[i]);
  }
  WELL("All the children read the number of lines");

  return 0;
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

int count_lines_in_file(char *file_name) {
  FILE *file;
  int n = 1, c;

  file = fopen(file_name, "r");

  if (!file)
    return -1;

  for (c = fgetc(file); c != EOF; c = fgetc(file))
    if (c == '\n')
      n++;
  fclose(file);

  return n;
}

int parent_loop(Parent *r) {
  return parent_loop_backend(r);  /* sorry */
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
