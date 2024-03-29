#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include <semaphore.h>
#include "../parent/interface.h"

int testable_read_file_segment(const Parent *parent, char *buf, unsigned long segment);

#endif
