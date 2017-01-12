#ifndef __CCUTILS_STD_H__
#define __CCUTILS_STD_H__

#include "ccutils/log.h"

#ifndef container_of
#define container_of(ptr, type, member) ({ \
		const typeof( ((type *)0)->member) *__mptr = (ptr); \
		(type *)( (char *)__mptr - (unsigned int)(((type *)0)->member));})
#endif

#endif
