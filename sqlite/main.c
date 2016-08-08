#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

int exec_cb(void *params, int ncol, char **col_val, char **col_name)
{
	int i;

	printf("call back:\n");

	for(i=0; i<ncol; i++){
		printf("\t%s", col_val[i]);
	}
	printf("\n");
	return 0;
}

int main(void)
{
	const char *sql = "create table mytable (id integer primary key, user integer)";
	const char *sql2 = "insert into mytable (id, user) values (NULL, 99)";
	const char *sql3 = "select * from mytable";
	char *errmsg = NULL;
	int ret;
	sqlite3 *db;

	ret = sqlite3_open("sqlite3-demo.db", &db);
	if(ret != SQLITE_OK){
		printf("cannot open db: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	printf("opened. \n");
	ret = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("1. cannot exec: %s\n", errmsg);
	}

	ret = sqlite3_exec(db, sql2, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("2. cannot exec: %s\n", errmsg);
	}
	ret = sqlite3_exec(db, sql3, exec_cb, NULL, &errmsg);
	if(ret != SQLITE_OK){
		printf("3. cannot exec: %s\n", errmsg);
	}

	sqlite3_free(errmsg);
	sqlite3_close(db);

	return 0;
}

