

int stockdb_init()
{
	char *cols[][2] = {
		{"code", "varchar(32)"},
		{"name", "varchar(32)"},
		{"date", "varchar(32)"},
		{"action", "varchar(32)"},
		{"price", "float"},
		{"cnterfee", "float"},	/* counter fee */
		{"tax", "float"},	/* stamp tax */
		{"tranfee", "float"},	/* transfer fee */
	};

	db_table_check();
	return 0;
}
