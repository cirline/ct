#define pr_fmt(fmt)	"main: " fmt
#include <ccutils/log.h>

#include "db_if.h"

int main(void)
{
	sqlite3 *db;
	int rc;

	rc = db_open("stocks.local.db", &db);
	if(rc != SQLITE_OK) {
		pr_err("open: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	stock_database_init(db);

	ui_insert_record(db);
	return 0;
}

