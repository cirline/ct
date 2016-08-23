#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "debug.h"
#include "shell.h"
#include "const.h"
#include "field.h"

void do_help(void)
{
	printf("this is help.\n");
}

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
			lp(DLI, "cmd[%d]: %s\n", i, sh->argv[i]);
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
	//int ret;
	int index;
	//const char *opstring = "";
	struct option commands[] = {
		[CMD_QUIT]			= {"quit", required_argument, 0, 0},
		[CMD_HELP]			= {"help", optional_argument, 0, 0},
		[CMD_FIELD_LIST]	= {"fl", no_argument, 0, 0},
		[CMD_FIELD_CREATE]	= {"fc", no_argument, 0, 0},
		[CMD_FIELD_UPDATE]	= {"fu", no_argument, 0, 0},
		[CMD_FIELD_GEN]		= {"fg", no_argument, 0, 0},
		[CMD_FIELD_DELETE]	= {"fd", no_argument, 0, 0},
		{NULL, 0, NULL, 0}
	};
	for(index = 0; index < END_CMDLIST; index++){
		if(strcmp(commands[index].name, sh->argv[0]) == 0)
			break;
	}
	switch(index){
		case CMD_QUIT:
			return SHELL_QUIT;
			break;
		case CMD_HELP:
			do_help();
			break;
		case CMD_FIELD_LIST:
		case CMD_FIELD_CREATE:
		case CMD_FIELD_UPDATE:
		case CMD_FIELD_GEN:
		case CMD_FIELD_DELETE:
			do_feild(index);
			break;
		default:
			err_command(sh->argv[0]);
			break;
	}
	/*
	while(-1 != (ret = getopt_long_only(sh->argc, sh->argv, opstring, options, &index))){
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
	*/
	lp(DLI, "command is %s\n", sh->argv[0]);

	return SHELL_OK;
}

void err_command(const char *cmd)
{
	printf("unknown command %s\n", cmd);
}
