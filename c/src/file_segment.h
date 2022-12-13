#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "parent.h"

char *testable_read_file_segment(const Parent *parent, unsigned long segment);

#endif
