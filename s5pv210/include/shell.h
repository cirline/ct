#ifndef __SHELL_H__
#define __SHELL_H__

#define	SHELL_MAX_ARGUMENTS	10

extern int shell_dump_registers(int argc, char *argv[]);
extern int shell_timer_1hz_buzz(int argc, char *argv[]);
extern int shell_sleep_test(int argc, char *argv[]);
extern int shell_lcd_test(int argc, char *argv[]);
extern int shell_flashleds_test(int argc, char *argv[]);
extern int shell_dump_sfrs(int argc, char *argv[]);
extern int shell_devmem(int argc, char *argv[]);

extern int shell_parse_arguments(char *str, char *argv[SHELL_MAX_ARGUMENTS]);

#endif

