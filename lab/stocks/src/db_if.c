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

	asprintf(&sql, "insert into history (code, name, date, action,"
			"price, volume, counter_fee, stamp_tax, transfer_fee)"
			"values ('%s', '%s', '%s', '%s', %f, %d, %f, %f, %f);",
			d->code, d->name, d->date, d->action,
			d->price, d->volume, d->counter_fee, d->stamp_tax, d->transfer_fee);

	printf("sql: %s\n", sql);
	rc = db_exec(db, sql, NULL, NULL, &errmsg);
	if(rc != SQLITE_OK) {
		pr_err("insert error: %s\n", errmsg);
		free(errmsg);
		return -1;
	}

	return 0;
}
