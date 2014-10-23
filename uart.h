#ifndef __UART_H__
#define __UART_H__

extern int uart_init(void);
extern void uart_wait_free(void);
extern int uart_send_char(char c);
extern int uart_send_string(char *s);
#endif
