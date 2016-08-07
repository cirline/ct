#ifndef __STOCK_UI_H__
#define __STOCK_UI_H__

int ui_insert_record(sqlite3 *db);
int ui_list_records(sqlite3 *db, char *code);

#endif
