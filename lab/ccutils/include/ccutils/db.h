#ifndef __CC_UTILS_H__
#define __CC_UTILS_H__

#include <ccutils/sqlite3.h>

enum e_celltype { CTYPE_INT, CTYPE_FLOAT, CTYPE_STRING };

struct celltype {
	enum e_celltype type;
	union {
		int n;
		float f;
		char *s;
	} data;
};

extern int db_open(const char *filename, sqlite3 **ppdb);
extern int db_exec(sqlite3 *hdl, const char *sql, int (*cb)(void*,int,char**,char**), void *arg, char **errmsg);

extern char * db_get_acell_int(sqlite3 *pdb, const char *tbl, const char *key, int keyval, const char *column);
extern char * db_get_acell_str(sqlite3 *pdb, const char *tbl, const char *key, const char *keyval, const char *column);
extern int db_table_check(sqlite3 *db, const char *tbl, char *cols[][2], const char *subsql);

#endif
