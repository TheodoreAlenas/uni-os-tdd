#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "../both/constants.h"
#include "get_names.h"

char *get_output_file_name(char *output_dir, unsigned child_index) {
  char *ans;
  ans = malloc(2 * MAX_FILE_NAME_LEN);
  sprintf(ans, "%s/index-%u-pid-%u", output_dir, child_index, getpid());
  return ans;
}
