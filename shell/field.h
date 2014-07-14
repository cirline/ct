#ifndef __UINFO_FIELD__
#define __UINFO_FIELD__

#include "const.h"
#include "list.h"

struct u_field{
	char desc[DESC_MAX_LEN];
	char name[FIELD_MAX_LEN];
	unsigned char encrypt;
	unsigned char addshow;
	unsigned char tableshow;
	unsigned char is_gen;
	struct list_head list;
};

extern int do_feild(cmd_t cmd);

#endif
