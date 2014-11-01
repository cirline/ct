#ifndef __IRQ_H__
#define __IRQ_H__

typedef enum {
	int_ext = 0,
} int_magic_t;

#define INTCLASSMASK	(3<<30)
#define EINT(x)			((int_ext << 30) + (x))

typedef int (*irq_func_t)(void);

extern int irq_init(int channel, irq_func_t func);
extern void inline eint_pending(int n, int index);
extern void irq_handler(void);

#endif
