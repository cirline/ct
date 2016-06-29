#ifndef __UART_H__
#define __UART_H__

#include "irq.h"

extern int uart_init(void);
extern void uart_wait_free(void);
extern int uart_send_char(char c);
extern int uart_send_string(char *s);
extern int uart_irq_init(irq_func_t func);
extern void uart_clear_int_pend(int n);
extern unsigned char uart_get_char(void);
#endif
