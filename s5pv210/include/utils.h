#ifndef __INC_COMMON_H__
#define __INC_COMMON_H__

#include "../common.h"

#define ARRAY_SIZE(arr)		(sizeof(arr)/sizeof(arr[0]))

#define memset(p, c, size)	s5p_memset((p), (c), (size))

extern int str2hl(const char *ss);
extern void inline s5p_memset(void *p, char c, unsigned long size);

#endif
