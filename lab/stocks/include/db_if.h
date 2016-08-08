#ifndef __STOCKS_DB_IF_H__
#define __STOCKS_DB_IF_H__

#include <ccutils/db.h>

#define COUNTER_FEE_RATE	0.0005

typedef int (*exec_cb_t)(void *param, int ncol, char **cval, char **cname);

struct hist_data {
	int id;
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

struct hist_data_analysis {
	int keep_volume;
	float total_cost;
	float total_divi;
	float cost_price;
};

struct hist_data_ui {
	char *cname;
	int slen;
};

int stock_database_init(sqlite3 *db);
int stock_insert_record(sqlite3 *db, struct hist_data *d);
int stock_update_record(sqlite3 *db, struct hist_data *d);
int stock_get_records(sqlite3 *db, char *code, exec_cb_t cb, void *param);
int stock_get_record_byid(sqlite3 *db, int id, exec_cb_t cb, void *param);
int stock_delete_record(sqlite3 *db, int id);

#endif
