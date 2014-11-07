#ifndef __IRQ_H__
#define __IRQ_H__

#define ICM_OFFSET		(32 - 4)
#define INTCLASSMASK	(0x3<<ICM_OFFSET)
#define INTMINORMASK	(~INTCLASSMASK)
#define MINOR_RTCALM	0xff
typedef enum {
	int_ext		= 0x0 << ICM_OFFSET,
	int_timer	= 0x1 << ICM_OFFSET,
} int_magic_t;

#define EINT(x)			(int_ext + ((x) & INTMINORMASK))
#define TIMERINT(x)		(int_timer + ((x) & INTMINORMASK))
#define RTCALMINT()		(int_timer + MINOR_RTCALM)

typedef int (*irq_func_t)(void);

extern int irq_init(int channel, irq_func_t func);
extern void inline eint_pending(int n, int index);
extern void inline tint_pending(int n);
extern void irq_handler(void);

#endif
