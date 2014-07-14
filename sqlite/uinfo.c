#include <stdio.h>

#include "sqlite3.h"


int uinfo(void)
{
	const char *sql = "create table t( \
					   id int(16) NOT NULL AUTOINCREMENT, \
					   data varchar(256), \
					   PRIMARY KEY(id) \
					   )";
	int ret;
	char *errmsg;
	sqlite3 *db;

	ret = sqlite3_open("sqlite.db", &db);
	if(ret != SQLITE_OK){
		printf("open db err: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	printf("db opened.\n");
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("exec err: %s\n", errmsg);
	}

	sqlite3_free(errmsg);
	sqlite3_close(db);

	return 0;
}


