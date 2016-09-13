#ifndef __SINAJS_H__
#define __SINAJS_H__

#define HTTP_FORMAT_STRING	"var hq_str_%s=\"%s,%f,%f,%f,%f,%f,%f,%f,%ld,%ld,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%s,%s,%s\";"

#define HTTP_ARGS_LIST(p)	\
	(p)->code,(p)->name,&(p)->open,&(p)->pre_close,&(p)->price,&(p)->high,&(p)->low,\
	&(p)->bid,&(p)->ask,&(p)->volume,&(p)->amount,\
	&(p)->b1_v,&(p)->b1_p,&(p)->b2_v,&(p)->b2_p,&(p)->b3_v,&(p)->b3_p,&(p)->b4_v,&(p)->b4_p,&(p)->b5_v,&(p)->b5_p,\
	&(p)->a1_v,&(p)->a1_p,\
	&(p)->a2_v,&(p)->a2_p,\
	&(p)->a3_v,&(p)->a3_p,\
	&(p)->a4_v,&(p)->a4_p,\
	&(p)->a5_v,&(p)->a5_p,\
	(p)->date,(p)->time,(p)->reserve

#define QTEST	"ab\"cd\"ef"

struct sinajs {
	char code[16];
	char name[20];
	float open, pre_close;
	float price, high, low;
	float bid, ask;
	long volume, amount;
	int b1_v, b2_v, b3_v, b4_v, b5_v;
	float b1_p, b2_p, b3_p, b4_p, b5_p;
	int a1_v, a2_v, a3_v, a4_v, a5_v;
	float a1_p, a2_p, a3_p, a4_p, a5_p;
	char date[16];
	char time[16];
	char reserve[16];
};

#endif
