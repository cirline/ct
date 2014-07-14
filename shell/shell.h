#ifndef __UINFO_SHELL__
#define __UINFO_SHELL__

#include "const.h"

typedef enum shell_code{
	SHELL_QUIT = 0,
	SHELL_OK,
	SHELL_FAIL,
	SHELL_EMPTY,

} shcode_t;

typedef struct u_shell {
	int argc;
	char *argv[CMD_MAX_NUM];
	char arg_buf[CMD_MAX_NUM][CMD_MAX_LEN];
} shell_t;

extern int shell_init_arg_array(shell_t *sh);
extern int shell_split_cmdline(shell_t *sh, char *s);
extern shcode_t shell_router(shell_t *sh);

#endif
