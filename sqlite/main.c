#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "sqlite3.h"
#include "field.h"
#include "shell.h"

int main(void)
{
	const char *sql = "create table mytable (id int primary key, user text)";
	char *errmsg = NULL;
	int ret;
	sqlite3 *db;
	char cmdin[128] = "c -";
	unsigned int cmd_prefix_len;
	char *pcmd;
	shell_t sh;

	shell_init_buf(&sh);
	cmd_prefix_len = strlen(cmdin);
	do{
		printf("> ");
		pcmd = cmdin + cmd_prefix_len;
		while((*pcmd++ = getc(stdin)) != '\n');
		*(--pcmd) = '\0';
		dp("cmdin: %s\n", cmdin);
		shell_split_str(&sh, cmdin);
	}while(SHELL_QUIT != shell_router(&sh));

	ret = sqlite3_open("sqlite3-demo.db", &db);
	if(ret != SQLITE_OK){
		printf("cannot open db: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	printf("opened. \n");
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("cannot exec: %s\n", errmsg);
	}

	sqlite3_free(errmsg);
	sqlite3_close(db);

	return 0;
}

