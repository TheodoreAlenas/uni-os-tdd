#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"
#include "msg_cycler.h"

typedef struct {
  unsigned child;
  int segment;
} Req;

char *testable_read_file_segment(const Parent *parent, unsigned long segment);


#endif
