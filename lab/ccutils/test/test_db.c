#define DEBUG
#define pr_fmt(fmt)	"dbtest: " fmt

#include <ccutils/db.h>
#include <ccutils/log.h>

int exec_cb(void *p, int nc, char **cv, char **cn)
{
	int i;

	pr_here();

	for(i = 0; i < nc; i++) {
		printf("\t%s - %s", cn[i], cv[i]);
	}
	printf("\n");

	return 0;
}

int main(void)
{

	const char *sql = "create table mytable (id integer primary key autoincrement, user integer)";
	const char *sql1 = "insert into mytable (user) values (48)";
	const char *sql2 = "select * from mytable";
	sqlite3 *db;
	int rc;
	char * errmsg = NULL;

	char *columns[][2] = {
		{"cola", "varchar(32)" },
		{"name", "integer" },
		{}
	};

	rc = db_open("test.local.db", &db);
	if(rc != SQLITE_OK) {
		pr_err("open %s\n", sqlite3_errmsg(db));
		return rc;
	}

	rc = db_exec(db, sql, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("create %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	rc = db_exec(db, sql1, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("insert %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	rc = db_exec(db, sql2, exec_cb, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("insert %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	db_table_check(db, "mytable", columns, NULL);
	db_table_check(db, "mytable1", NULL, NULL);

	sqlite3_close(db);

	return rc;
}

