#define _GNU_SOURCE
#include <stdlib.h>

#include <ccutils/db.h>
#include <ccutils/log.h>

#include "db_if.h"

int stock_database_init(sqlite3 *db)
{
	char * cols[][2] = {
		{"code", "varchar(32)"},
		{"name", "varchar(32)"},
		{"date", "varchar(32)"},
		{"action", "varchar(32)"},
		{"price", "float"},
		{"volume", "integer"},
		{"counter_fee", "float"},
		{"stamp_tax", "float"},
		{"transfer_fee", "float"},
		{},
	};

	db_table_check(db, "history", cols, NULL);

	return 0;
}

int stock_insert_record(sqlite3 *db, struct hist_data *d)
{
	char *sql;
	char *errmsg;
	int rc;

	rc = asprintf(&sql, "insert into history (code, name, date, action,"
			"price, volume, counter_fee, stamp_tax, transfer_fee)"
			"values ('%s', '%s', '%s', '%s', %.3f, %d, %.3f, %.3f, %.3f);",
			d->code, d->name, d->date, d->action,
			d->price, d->volume, d->counter_fee, d->stamp_tax, d->transfer_fee);
	if(rc < 0) {
		pr_err("asprintf error\n");
		return -1;
	}

	printf("sql: %s\n", sql);
	rc = db_exec(db, sql, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("insert error: %s\n", errmsg);
		free(errmsg);
		rc = -1;
	}
	free(sql);

	return rc;
}

int stock_update_record(sqlite3 *db, struct hist_data *d)
{
	char *sql;
	char *errmsg;
	int rc;

	rc = asprintf(&sql, "update history set code = '%s', name = '%s', date = '%s', action = '%s', "
			"price = %.3f, volume = %d, counter_fee = %.3f, stamp_tax = %.3f, transfer_fee = %.3f "
			"where id = %d;",
			d->code, d->name, d->date, d->action,
			d->price, d->volume, d->counter_fee, d->stamp_tax, d->transfer_fee,
			d->id);
	if(rc < 0) {
		pr_err("%s, asprintf error\n", __func__);
		return -1;
	}
	rc = db_exec(db, sql, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("update fail: %s\n", errmsg);
		sqlite3_free(errmsg);
		rc = -1;
	}
	free(sql);

	return rc;
}

int stock_get_records(sqlite3 *db, char *code, int (*cb)(void *, int, char**, char**), void *param)
{
	char *sql;
	char *errmsg;
	int rc;

	if(code)
		rc = asprintf(&sql, "select * from history where code = '%s' order by date;", code);
	else
		rc = asprintf(&sql, "select * from history order by date;");

	if(rc < 0) {
		pr_err("%s, asprintf sql error\n", __func__);
		return -1;
	}

	rc = db_exec(db, sql, cb, param, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("%s, get stocks error: %s\n", errmsg);
		free(errmsg);
		rc = -1;
	}
	free(sql);

	return rc;
}

int stock_get_record_byid(sqlite3 *db, int id, exec_cb_t cb, void *param)
{
	char *sql;
	char *errmsg;
	int rc;

	rc = asprintf(&sql, "select * from history where id = %d;", id);
	if(rc < 0) {
		pr_err("%s, asprintf error\n", __func__);
		return -1;
	}

	rc = db_exec(db, sql, cb, param, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("%s, select error: %s\n", __func__, errmsg);
		free(errmsg);
		rc = -1;
	}
	free(sql);

	return rc;
}

int stock_delete_record(sqlite3 *db, int id)
{
	char *sql;
	char *errmsg;
	int rc;

	rc = asprintf(&sql, "delete from history where id = %d;", id);
	if(rc < 0) {
		pr_err("%s, asprintf error\n", __func__);
		return -1;
	}

	rc = db_exec(db, sql, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("delete error: %s", errmsg);
		free(errmsg);
		rc = -1;
	}
	free(sql);

	return rc;
}
