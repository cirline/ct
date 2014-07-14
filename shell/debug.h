#ifndef __UINFO_DEBUG__
#define __UINFO_DEBUG__

#define DEBUG

#define DLI		0x0001
#define DLW		0x0011
#define DLE		0x0111

#define DL_NONE 0x0000
#define DL_INFO	0x0001
#define DL_WARN	0x0010
#define DL_ERR	0x0100

#define DEBUG_LEVEL DL_WARN

#ifdef DEBUG
#define lp(level, format, arg...) do{ if(level & DEBUG_LEVEL) \
	printf(format, ##arg); \
}while(0);
#else
#define lp(level, format, arg...)
#endif



#endif
