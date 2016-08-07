#define pr_fmt(fmt)	"main: " fmt

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <ccutils/log.h>
#include <ccutils/io.h>

#include "db_if.h"
#include "ui.h"

static const struct option longopts[] = {
	{"add", no_argument, NULL, 'a'},
	{"list", no_argument, NULL, 'l'},
	{"rm", required_argument, NULL, 'r'},
	{"debug", no_argument, NULL, 'd'},
	{NULL, 0, NULL, 0}
};

enum stock_action {
	ACTION_ADD = 0,
	ACTION_LIST,
	ACTION_RM,

	ACTION_NULL,
};

int main(int argc, char *argv[])
{
	sqlite3 *db;
	int rc;
	int optc;
	int lose = 0;
	enum stock_action action = ACTION_LIST;
	int rmid;
	int debug = 0;
	char db_uptodate[16];

	while((optc = getopt_long(argc, argv, "alr:d", longopts, NULL)) != -1) {
		switch(optc) {
			case 'a':
				action = ACTION_ADD;
				break;
			case 'l':
				action = ACTION_LIST;
				break;
			case 'r':
				action = ACTION_RM;
				rmid = atoi(optarg);
				break;
			case 'd':
				debug = 1;
				break;
			default:
				lose = 1;
		}
	}

	if(lose || optind < argc) {
		pr_err("arg error\n");
	}

	if(!debug) {
		io_getdata("database is update(yes/no)? ", "%s", 1, db_uptodate);
		if(strcmp(db_uptodate, "yes")) {
			pr_err("please update database!\n");
			return -1;
		}
	}

	rc = db_open(debug ? "stocks.local.db" : "stock.db", &db);
	if(rc != SQLITE_OK) {
		pr_err("open: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	stock_database_init(db);

	switch(action) {
		case ACTION_ADD:
			ui_insert_record(db);
			break;
		case ACTION_LIST:
			ui_list_records(db, NULL);
			break;
		case ACTION_RM:
			ui_delete_record(db, rmid);
			break;
		default:
			pr_err("unkown action %d\n", action);

	}

	return 0;
}

