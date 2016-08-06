#include <stdio.h>
#include <string.h>

#include <ccutils/log.h>

#include "db_if.h"

int ui_insert_record(sqlite3 *db)
{
	struct hist_data dat;
	struct hist_data *data = &dat;
	int rc;
	int len;
	int a, b, c;

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

	do {
		printf("price: ");
		rc = scanf("%f", &data->price);
	} while(rc != 1);

	do {
		printf("volume: ");
		rc = scanf("%d", &data->volume);
	} while(rc != 1);

	do {
		printf("counter_fee: ");
		rc = scanf("%f", &data->counter_fee);
	} while(rc != 1);

	do {
		printf("stamp_tax: ");
		rc = scanf("%f", &data->stamp_tax);
	} while(rc != 1);

	do {
		printf("transfer_fee: ");
		rc = scanf("%f", &data->transfer_fee);
	} while(rc != 1);

	stock_insert_record(db, data);

	return 0;
}

