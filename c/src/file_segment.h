#ifndef FILE_SEGMENT_H
#define FILE_SEGMENT_H

#include "parent.h"

char *parent_read_file_segment(const Parent *parent, unsigned long segment);
char *read_segment_from_open_file(const ParentParams *pp, FILE *file, unsigned long segment);
int skip_to_segment(FILE *file, unsigned long segment, unsigned long segment_length);
void append_to_final(char **to_return, FILE *file);

#endif
