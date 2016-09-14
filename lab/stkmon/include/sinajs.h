#ifndef __SINAJS_H__
#define __SINAJS_H__

struct sinajs {
	char code[16];
	char name[20];
	float open, pre_close;
	float price, high, low;
	float bid, ask;
	long volume;
	long amount;
	int bv[5];
	float bp[5];
	int av[5];
	float ap[5];
	char date[16];
	char time[16];
	char reserve[16];
};

#endif
