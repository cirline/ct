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
	int i;

	shell_init_arg_array(&sh);
	sh.argc = argc;
	for(i=0; i<argc; i++){
		sh.argv[i] = argv[i];
	}
	shell_router(&sh);

	do{
		printf("> ");
		pcmd = cmdin;
		while((*pcmd++ = getc(stdin)) != '\n');
		*(--pcmd) = '\0';
		dp("cmdin: %s\n", cmdin);
		shell_split_cmdline(&sh, cmdin);
	}while(SHELL_QUIT != shell_router(&sh));

	return 0;
}

