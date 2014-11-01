#ifndef __IRQ_H__
#define __IRQ_H__

typedef enum {
	int_ext = 0,
} int_magic_t;

#define EINT(x)		((int_ext << 30) + (x))

typedef void (*irq_func_t)(void);

extern int irq_init(int channel, irq_func_t func);
extern void irq_handler(void);

#endif
