#define DEBUG
#define pr_fmt(fmt)	"ui: " fmt

#include <stdio.h>
#include <string.h>

#include <ccutils/log.h>
#include <ccutils/io.h>

#include "db_if.h"

static struct hist_data_ui dui[] = {
	{"id", 4},
	{"code", 8},
	{"name", 12},
	{"date", 12},
	{"action", 8},
	{"price", 8},
	{"volume", 8},
	{"counter_fee", 12},
	{"stamp_tax", 10},
	{"transfer_fee", 14},
	{NULL, 0}
};

int ui_insert_record(sqlite3 *db)
{
	struct hist_data dat;
	struct hist_data *data = &dat;
	int rc;
	int len;
	int a, b, c;
	char confirm[16];

	do {
		printf("code: ");
		rc = scanf("%s", data->code);
	} while(rc != 1 || strlen(data->code) != 6);

	do {
		printf("name: ");
		rc = scanf("%s", data->name);
	} while(rc != 1);

	do {
		printf("date: ");
		rc = scanf("%s", data->date);
	} while (rc != 1 || strlen(data->date) != (4 + 1 + 2 + 1 + 2));

	do {
		printf("action(buy, sell, divi): ");
		rc = scanf("%s", data->action);
		a = strcmp(data->action, "buy");
		b = strcmp(data->action, "sell");
		c = strcmp(data->action, "divi");
	} while (rc != 1 || (a && b && c));

	io_getdata("price: ", "%f", 1, &data->price);
	io_getdata("volume: ", "%d", 1, &data->volume);
	io_getdata("counter_fee: ", "%f", 1, &data->counter_fee);
	io_getdata("stamp_tax: ", "%f", 1, &data->stamp_tax);
	io_getdata("transfer_fee: ", "%f", 1, &data->transfer_fee);

	printf("Confirm:\n");
	printf(" %16s: %s\n", "code", data->code);
	printf(" %16s: %s\n", "name", data->name);
	printf(" %16s: %s\n", "date", data->date);
	printf(" %16s: %s\n", "action", data->action);
	printf(" %16s: %.2f\n", "price", data->price);
	printf(" %16s: %d\n", "volume", data->volume);
	printf(" %16s: %.2f\n", "counter fee", data->counter_fee);
	printf(" %16s: %.2f\n", "stamp tax", data->stamp_tax);
	printf(" %16s: %.2f\n", "transfer fee", data->transfer_fee);

	do {
		printf("\nRight(yes/no) ? ");
		rc = scanf("%s", confirm);
	} while(rc != 1);

	if(strcmp(confirm, "yes"))
		return -1;

	rc = stock_insert_record(db, data);
	if(rc < 0){
		pr_err("insert error\n");
		return -1;
	}
	pr_info("insert success\n");

	return 0;
}

void ui_record_header(void)
{
	int i;

	for(i = 0; dui[i].cname; i++)
		printf("%*s", dui[i].slen, dui[i].cname);
	printf("\n");
}

static int ui_list_records_callback(void *param, int nc, char **cv, char **cn)
{
	int i;

	for(i = 0; i < nc; i++)
		printf("%*s", dui[i].slen, cv[i]);
	printf("\n");

	return 0;
}

int ui_list_records(sqlite3 *db, char *code)
{
	if(code && (strlen(code) != 6)) {
		pr_err("error code %s\n", code);
	}

	ui_record_header();

	stock_get_records(db, code, ui_list_records_callback, NULL);

	return 0;
}

