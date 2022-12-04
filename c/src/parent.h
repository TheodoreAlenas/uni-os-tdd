#include <stdio.h>

#include "stack.h"
#include "child_data.h"


typedef struct main_resources {
  Stack *requests;
  ChildData *children;
} MainResources;

MainResources *create_resources_of_main(int num_of_workers);
int main_loop(MainResources *r);
void free_resources_of_main(MainResources *r);


