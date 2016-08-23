#ifndef __STOCK_UI_H__
#define __STOCK_UI_H__

int ui_list_records(sqlite3 *db, char *code);
int ui_insert_record(sqlite3 *db);
int ui_delete_record(sqlite3 *db, int id);
int ui_edit_record(sqlite3 *db, int id);

#endif
