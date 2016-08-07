#define pr_fmt(fmt)	"main: " fmt

#include <getopt.h>

#include <ccutils/log.h>

#include "db_if.h"
#include "ui.h"

static const struct option longopts[] = {
	{"add", no_argument, NULL, 'a'},
	{"list", no_argument, NULL, 'l'},
	{NULL, 0, NULL, 0}
};

int main(int argc, char *argv[])
{
	sqlite3 *db;
	int rc;
	int optc;
	int lose = 0;

	rc = db_open("stocks.local.db", &db);
	if(rc != SQLITE_OK) {
		pr_err("open: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	stock_database_init(db);

	while((optc = getopt_long(argc, argv, "al", longopts, NULL)) != -1) {
		switch(optc) {
			case 'a':
				ui_insert_record(db);
				break;
			case 'l':
				ui_list_records(db, NULL);
				break;
			default:
				lose = 1;
				break;
		}
	}

	if(lose || optind < argc) {
		pr_err("arg error\n");
	}

	return 0;
}

