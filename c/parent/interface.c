#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "interface.h"
#include "../both/dev_mode.h"
#include "../both/constants.h"
#include "loop.h"
#include "../both/shmem.h"
#include "../both/req.h"

sem_t *init_sem(const Parent *r);
sem_t **open_child_created_sems(const Parent *r);
int count_lines_in_file(char *file_name);
void opening_ceremony(Parent *r);

int parent_init(Parent *r) {
  int i;

  WELLL(printf("%s, %s", r->shmem_name_yes_please, r->shmem_name_youre_ready));
  r->shmem_yes_please = shmem_create_i_want(r);
  r->shmem_youre_ready = shmem_create_thank_you(r);

  r->lines_in_file = count_lines_in_file(r->input_file);
  if (r->lines_in_file == -1)
    return -1;

  r->sems_youre_ready = open_child_created_sems(r);
  r->sem_yes_please = init_sem(r);
  if (r->sem_yes_please == NULL)
    return -1;

  opening_ceremony(r);

  return 0;
}

void parent_free(Parent *r) {
  int i;
  WELL("(not freeing ParentParams)");

  if (r->sem_yes_please) {
    sem_unlink(r->sem_name_yes_please);
    sem_close(r->sem_yes_please);
  }
  for (i = 0; i < r->num_of_children; i++) {
    if (r->sems_youre_ready[i])
      sem_close(r->sems_youre_ready[i]);
  }
  free(r->sems_youre_ready);

  shm_unlink(r->shmem_name_yes_please);
  shm_unlink(r->shmem_name_youre_ready);
}

void post_all(Parent *r) {
  int i;
  for (i = 0; i < r->num_of_children; i++)
    sem_post(r->sems_youre_ready[i]);
}

void wait_all(Parent *r) {
  int i;
  for (i = 0; i < r->num_of_children; i++)
    sem_wait(r->sem_yes_please);
}

/* for README, opening-ceremony */
void opening_ceremony(Parent *r) {
  *( (int *) r->shmem_youre_ready ) = r->lines_in_file;

  WELL("broadcasting the total lines of the file");
  post_all(r);

  WELL("waiting until they read the number of lines");
  wait_all(r);

  WELL("signaling that they may start requesting");
  post_all(r);
}
/* end of snippet */

sem_t **open_child_created_sems(const Parent *r) {
  int i;
  sem_t **to_return;
  char msg[128], sem_name[MAX_FILE_NAME_LEN];

  to_return = malloc(r->num_of_children * sizeof(sem_t*));

  for (i = 0; i < r->num_of_children; i++) {
    /* the parent comes second, so the semaphore may be opened either way */
    sprintf(sem_name, "%s-%d", r->sem_name_youre_ready_template, i);
    to_return[i] = sem_open(sem_name, O_CREAT | O_RDONLY, 0666, 0);
    WELL(sem_name);

    if (to_return[i] == NULL) {
      sprintf(msg, "parent trying to open child %d's '%s' created semaphore",
          i, sem_name);
      perror(msg);
    }
  }

  return to_return;
}

sem_t *init_sem(const Parent *r) {
  sem_t *s;
  unsigned i;

  s = sem_open(r->sem_name_yes_please, O_CREAT | O_WRONLY, 0666, 0);
  if (s == NULL) {
    perror("parent's 'yes please' semaphore");
    return NULL;
  }

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

int parent_waitpid(const Parent *r) {
  int i, status;

  for (i = 0; i < r->num_of_children; i++) {
    WELLL(printf("waiting child the %d-th time", i));
    waitpid(-1, &status, 0);
  }

  if (WEXITSTATUS(status))
    return 0;
  return -1;
}
