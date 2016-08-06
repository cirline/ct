#ifndef __STOCKS_DB_IF_H__
#define __STOCKS_DB_IF_H__

#include <ccutils/db.h>

struct hist_data {
	char code[8];
	char name[16];
	char date[16];
	char action[16];
	float price;
	int volume;
	float counter_fee;
	float stamp_tax;
	float transfer_fee;
};

int stock_database_init(sqlite3 *db);

#endif
