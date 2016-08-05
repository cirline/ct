#include <stdlib.h>

#include <ccutils/db.h>

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
