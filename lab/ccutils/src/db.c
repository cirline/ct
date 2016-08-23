//#define DEBUG

#define _GNU_SOURCE
#define pr_fmt(fmt)	"db: " fmt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ccutils/db.h>
#include <ccutils/log.h>

//int check_table_and_create()
//

/**
 * db_open - sqlite3_open
 * @filename: database filename (UTF-8)
 * @ppdb: SQLite db handle
 *
 * the return value is a integer equal SQLITE_OK at opened successfully,
 * otherwise an [error code] is returned,
 * use sqlite3_errmsg(ppdb) obtain an error description
 */
int db_open(const char *filename, sqlite3 **ppdb)
{
	return sqlite3_open(filename, ppdb);
}

/**
 * db_exec - sqlite3_exec
 * @hdl: an opened database
 * @sql: SQL to be evaluated
 * @cb : callback function
 * @arg: 1st argument to callback
 * @errmsg: error msg written here, invoke sqlite3_free() to free the malloc memory
 *
 * int (*cb)(void *param, int ncolumn, char **column_val, char **column_name)
 * if cb is not NULL, then it is invoked for each result row
 *
 * the return value is integer equal SQLite_ success/failure codes
 * also write an error message into errmsg
 * if callback returns non-zero, db_exec will break and return SQLITE_ABORT
 */
int db_exec(sqlite3 *hdl, const char *sql, int (*cb)(void*,int,char**,char**), void *arg, char **errmsg)
{
	return sqlite3_exec(hdl, sql, cb, arg, errmsg);
}

static int db_get_acell_callback(void *p, int nc, char **cv, char **cn)
{
	int i;
	char **cellval = p;

	*cellval = nc > 0 ? strdup(cv[0]) : NULL;

	return 0;
}

static char * db_get_acell(sqlite3 *pdb, const char *sql)
{
	char *msg;
	int rc;
	char *cellval = NULL;

	if(!pdb)
		return NULL;

	pr_here();
	rc = db_exec(pdb, sql, db_get_acell_callback, &cellval, &msg);
	if(rc != SQLITE_OK) {
		pr_err("exec: %s\n", msg);
		free(msg);
	}

	return cellval;
}

char * db_get_acell_int(sqlite3 *pdb, const char *tbl, const char *key, int keyval, const char *column)
{
	char *sql;
	char *result;

	pr_here();
	asprintf(&sql, "select %s from '%s' where %s = %d;", column, tbl, key, keyval);
	result = db_get_acell(pdb, sql);
	free(sql);

	return result;
}

/**
 * db_get_acell_str - get a cell value by record key and column name
 * @db: an opened database
 * @tbl: table name
 * @key: record key column name
 * @keyval: record key value
 * @column: cell column name
 *
 * the return value is a string by strdup from cell value, invoke free() to release the malloc memory
 * or NULL if record not found
 */
char * db_get_acell_str(sqlite3 *pdb, const char *tbl, const char *key, const char *keyval, const char *column)
{
	char *sql;
	char *result;

	asprintf(&sql, "select %s from '%s' where %s = '%s';", column, tbl, key, keyval);
	result = db_get_acell(pdb, sql);
	free(sql);

	return result;
}

/**
 * db_column_index - get column index by name
 * @db: an opened database
 * @tbl: table name
 * @column: column name
 *
 * the return value is a integer of 1 to Ncolumn-1,
 * zero is column "id" but never return, and -1 if column not found
 */
int db_column_index(sqlite3 *db, const char *tbl, const char *column)
{
	char *sql_ct;
	char *scol;
	char *anchor;
	int index;

	if(!db)
		return -1;

	sql_ct = db_get_acell_str(db, "sqlite_master", "name", tbl, "sql");

	asprintf(&scol, ", %s ", column);
	anchor = strstr(sql_ct, scol);
	pr_debug("anchor %s,,,, %p, %p\n", anchor, sql_ct, anchor);
	if(!anchor) {
		index = -1;
	} else {
		char *temp = sql_ct;
		for(index = 1; temp != anchor; temp++) {
			if(*temp == ',')
				index++;
		}
	}

	free(scol);
	free(sql_ct);

	return index;
}

/**
 * db_column_check - check table columns and creat it
 * @db: an opened database
 * @tbl: table name
 * @column: column group
 *
 * the return value is a integer greater than zero if column exist or create successfully,
 * or less then zero if column not exist or create failure.
 */
int db_column_check(sqlite3 *db, const char *tbl, const char *column[])
{
	int idx;
	
	idx = db_column_index(db, tbl, column[0]);

	if(idx < 0) {
		int rc;
		char *errmsg;
		char *sql;

		asprintf(&sql, "alter table '%s' add column %s %s;", tbl, column[0], column[1]);
		rc = db_exec(db, sql, NULL, NULL, &errmsg);
		if(rc != SQLITE_OK) {
			pr_err("alter %s,%s: %s\n", tbl, column[0], errmsg);
			free(errmsg);
			rc = -1;
		} else
			rc = 1;
	}

	return idx;
}

/**
 * db_table_check - check table and creat it
 * @db: an opened db
 * @tbl: table name
 * @cols: column group
 * @subsql: use subsql to create unique or no null column
 */
int db_table_check(sqlite3 *db, const char *tbl, char *cols[][2], const char *subsql)
{
	char *sql;
	char *result;
	char *msg;
	int rc;

	result = db_get_acell_str(db, "sqlite_master", "name", tbl, "*");
	if(result) {
		pr_info("%s found\n", tbl);
		free(result);
	} else {
		pr_warn("%s not found\n", tbl);
		asprintf(&sql, "create table '%s' (id integer primary key autoincrement %s);", tbl, subsql ? subsql : "");
		pr_info("sql: %s\n", sql);
		rc = db_exec(db, sql, NULL, NULL, &msg);
		if(rc != SQLITE_OK) {
			pr_err("create table: %s\n", msg);
			return -1;
		}
	}

	if(cols) {
		int i;

		for(i = 0; cols[i][0]; i++) {
			db_column_check(db, tbl, cols[i]);
		}
	}

	return 0;
}

