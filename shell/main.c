#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "field.h"
#include "shell.h"
#include "const.h"

int main(int argc, char *argv[])
{
	char cmdin[128];
	char *pcmd;
	shell_t sh;

	shell_init_arg_array(&sh);
	do{
		printf("> ");
		pcmd = cmdin;
		while((*pcmd++ = getc(stdin)) != '\n');
		*(--pcmd) = '\0';
		lp(DLI, "cmdin: %s\n", cmdin);
		shell_split_cmdline(&sh, cmdin);
	}while(SHELL_QUIT != shell_router(&sh));
	printf("bye!\n");

	return 0;
}

