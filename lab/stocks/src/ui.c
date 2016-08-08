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
	{"date", 10},
	{"action", 8},
	{"price", 12},
	{"volume", 8},
	{"counter_fee", 12},
	{"stamp_tax", 10},
	{"transfer_fee", 14},
	{NULL, 0}
};

void ui_show_record_detail(char *title, struct hist_data *data)
{
	printf("%s\n", title);

	printf(" %16s: %d\n", "id", data->id);
	printf(" %16s: %s\n", "code", data->code);
	printf(" %16s: %s\n", "name", data->name);
	printf(" %16s: %s\n", "date", data->date);
	printf(" %16s: %s\n", "action", data->action);
	printf(" %16s: %.2f\n", "price", data->price);
	printf(" %16s: %d\n", "volume", data->volume);
	printf(" %16s: %.2f\n", "counter fee", data->counter_fee);
	printf(" %16s: %.2f\n", "stamp tax", data->stamp_tax);
	printf(" %16s: %.2f\n", "transfer fee", data->transfer_fee);
}

static int ui_get_hist_data(struct hist_data *data)
{
	int rc;
	int len;
	int a, b, c;
	char confirm[16];
	int input[16];
	char tipbuf[1024];

	sprintf(tipbuf, "code(%s): ", data->code);
	io_getdata(tipbuf, "%s", 6, data->code);

	sprintf(tipbuf, "name(%s): ", data->name);
	io_getdata(tipbuf, "%s", 0, data->name);

	sprintf(tipbuf, "date(%s): ", data->date);
	io_getdata(tipbuf, "%s", 8, data->date);

	do {
		sprintf(tipbuf, "action(buy, sell, divi)(%s): ", data->action);
		io_getdata(tipbuf, "%s", 0, data->action);
		a = strcmp(data->action, "buy");
		b = strcmp(data->action, "sell");
		c = strcmp(data->action, "divi");
	} while (a && b && c);

	sprintf(tipbuf, "price(%8.2f): ", data->price);
	io_getdata(tipbuf, "%f", 0, &data->price);

	sprintf(tipbuf, "volume(%d): ", data->volume);
	io_getdata(tipbuf, "%d", 0, &data->volume);

	sprintf(tipbuf, "counter_fee(%8.2f): ", data->counter_fee);
	io_getdata(tipbuf, "%f", 0, &data->counter_fee);

	sprintf(tipbuf, "stamp_tax(%8.2f): ", data->stamp_tax);
	io_getdata(tipbuf, "%f", 0, &data->stamp_tax);

	sprintf(tipbuf, "transfer_fee(%8.2f): ", data->transfer_fee);
	io_getdata(tipbuf, "%f", 0, &data->transfer_fee);

	data->id = 0;
	ui_show_record_detail("Confirm:", data);
	do {
		printf("\nRight(yes/no) ? ");
		rc = scanf("%s", confirm);
	} while(rc != 1);

	if(strcmp(confirm, "yes"))
		return -1;


	return 0;
}

int ui_insert_record(sqlite3 *db)
{
	struct hist_data data;
	int rc;

	memset(&data, 0, sizeof(struct hist_data));
	while(ui_get_hist_data(&data) < 0)
		pr_err("input again...\n");

	rc = stock_insert_record(db, &data);
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

static int ui_list_record_byid_callback(void *param, int nc, char **cv, char **cn)
{
	int i;

	for(i = 0; i < nc; i++)
		printf("%16s: %s\n", cn[i], cv[i]);
	printf("\n");

	return 0;
}

int ui_delete_record(sqlite3 *db, int id)
{
	char *sql;
	int rc;
	char confirm[16];

	if(!db || id < 0) {
		pr_err("%s, invalid value\n");
		return -1;
	}

	rc = stock_get_record_byid(db, id, ui_list_record_byid_callback);
	if(rc < 0) {
		pr_err("record id = %d not exist\n", id);
		return -1;
	}

	io_getdata("delete(yes/no) ?", "%s", 1, confirm);
	if(strcmp(confirm, "yes")) {
		pr_info("ignore\n");
		return -1;
	}
	rc = stock_delete_record(db, id);
	if(rc < 0) {
		pr_err("delete error\n");
		return -1;
	}

	pr_info("delete %d success\n", id);

	return 0;
}

