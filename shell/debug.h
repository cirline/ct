#ifndef __UINFO_DEBUG__
#define __UINFO_DEBUG__

#define DEBUG

#define DEBUG_MAIN

#ifdef DEBUG
#define dp(format, arg...) printf(format, ##arg)
#else
#define dp(format, arg...)
#endif



#endif
