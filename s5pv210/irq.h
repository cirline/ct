#ifndef __IRQ_H__
#define __IRQ_H__

/**
 * interrupt channel
 * -----------------
 * | class | minor |
 * -----------------
 */

/** interrupt class offset */
#define IC_OFFSET	(16)
#define IRQCLASSMASK	(((unsigned int)(-1)) >> IC_OFFSET)
#define IRQMINORMASK	((unsigned int)(1 << IC_OFFSET) - 1)
#define MINOR_RTCALM	0xff
typedef enum {
	ic_ext		= 0x0,
	ic_timer	= 0x1,
	ic_uart		= 0x2,
} irq_class_t;

#define IRQ_CHANNEL(cls, minor)	\
	((((cls) & IRQCLASSMASK) << IC_OFFSET) + ((minor) & IRQMINORMASK))

#define EINT(x)			IRQ_CHANNEL(ic_ext, x)
#define TIMERINT(x)		IRQ_CHANNEL(ic_timer, x)
#define RTCALMINT()		IRQ_CHANNEL(ic_timer, MINOR_RTCALM)
#define UARTINT(x)		IRQ_CHANNEL(ic_uart, x)

typedef int (*irq_func_t)(void);

extern int irq_init(unsigned int channel, irq_func_t func);
extern void irq_clear_pending(unsigned int channel);
extern void inline eint_pending(int n, int index);
extern void inline uint_pending(int n);
extern void irq_handler(void);

#endif
