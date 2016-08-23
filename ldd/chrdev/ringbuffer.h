#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <linux/ioctl.h>

#define _RB_DEBUG

#define RB_IOC_MAGIC    'k'
#define RB_IOC_GET_MEMSIZE      _IOWR(RB_IOC_MAGIC, 0, int)

#ifdef _RB_DEBUG
#else
#endif

#define eprintk(format, args...)        printk(KERN_ERR"ringbuffer_err[%d]: "format, __LINE__, ##args)

#endif

