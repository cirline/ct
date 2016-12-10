#ifndef __SINAJS_H__
#define __SINAJS_H__

#include "stkmon/stkmon.h"

#define SJS_CODE_SIZE	16
#define SJS_NAME_SIZE	20
#define SJS_DATE_SIZE	16
#define SJS_TIME_SIZE	16

struct sinajs {
	struct stk_stkdat	common;
	char code[SJS_CODE_SIZE];
	float open, pre_close;
	float price, high, low;
	float bid, ask;
	long volume;
	long amount;
	int bv[5];
	float bp[5];
	int av[5];
	float ap[5];
	char date[SJS_DATE_SIZE];
	char time[SJS_TIME_SIZE];
	char reserve[16];
};

struct sinajs_index {
	char	code	[SJS_CODE_SIZE];
	char	name	[SJS_NAME_SIZE];
	float	index;
	float	range;
	float	rate;
	long	volume;
	long	amount;
};

extern int sinajs_pull_data(struct stk_xmlcfg *sxc);
extern void sinajs_print(struct sinajs *sj);
extern int sinajs_decode(char *buffer, struct sinajs *sj);
int sinajs_apply_data(struct stk_xmlcfg *sxc);

#endif
