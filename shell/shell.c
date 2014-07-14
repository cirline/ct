#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "debug.h"
#include "shell.h"
#include "const.h"

/**
 * init the shell input buffer
 * argc
 * argv
 */
int shell_init_arg_array(shell_t *sh)
{
	int i;

	for(i=0; i<CMD_MAX_NUM; i++){
		sh->argv[i] = sh->arg_buf[i];
	}
	return 0;
}

/**
 * format the cmd input
 * translate tab to space
 * delete unnecessary space
 */
shcode_t shell_format_cmdline(char *s)
{
	char *tmp;

	tmp = strdup(s);
	while('\0' != *s){
	}
	free(tmp);

	return SHELL_OK;
}

/**
 * split the input string to argument array
 * argc
 * argv
 */
int shell_split_cmdline(shell_t *sh, char *s)
{
	char *cur = s;
	sh->argc = 0;

	while('\0' != *s){
		cur = s;
		if(' ' == *cur || '\t' == *cur){
			s = ++cur;
			continue;
		}
		while(' ' != *cur && '\t' != *cur && '\0' != *cur)
			cur++;
		if('\0' == *cur)
			break;
		*cur = '\0';
		strcpy(sh->argv[sh->argc++], s);
		s = ++cur;
	}
	if(*s != '\0')
		strcpy(sh->argv[sh->argc++], s);
#ifdef DEBUG
	{
		int i;
		for(i = 0; i < sh->argc; i++){
			printf("cmd[%d]: %s\n", i, sh->argv[i]);
		}
	}
#endif
	return 0;
}

/**
 * route the cmdline
 * argv[0] is the command
 * argv[1]... is the option and value
 */
shcode_t shell_router(shell_t *sh)
{
	int ret;
	int index;
	struct option options[] = {
		/* 0 */
		{"quit", required_argument, 0, 0},
		{"help", optional_argument, 0, 0},
		{"fl", no_argument, 0, 0},
		{"fc", no_argument, 0, 0},
		{"fu", no_argument, 0, 0},
		{"fg", no_argument, 0, 0},
		{"fd", no_argument, 0, 0},
		{0,0,0,0}
	};
	while(-1 != (ret = getopt_long_only(sh->argc, sh->argv, NULL, options, &index))){
		dp("index = %d\n", index);
		switch(index){
			case 0:
				return SHELL_QUIT;
				break;
			case 1:
				break;
			default:
				break;
		}
	}

	return SHELL_OK;
}


