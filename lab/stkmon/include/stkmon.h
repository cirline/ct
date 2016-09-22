#ifndef __STKMON_H__
#define __STKMON_H__

#include <gtk/gtk.h>

#define COLOR_RISE	"red"
#define COLOR_DROP	"blue"
#define COLOR_EQ	"black"

#define SM_DEFAULT_DELAY_MS	5
#define SM_DEFAULT_INTERVAL	5000

#define SM_CODE_SZ	16
#define SM_STKEX_SZ	8

struct sm_cfg {
	int delay_ms;
};

struct sm_stkui {
	GtkWidget *label_code;
	GtkWidget *label_price;
	GtkWidget *label_raise;
};

struct sm_stock {
	char code[SM_CODE_SZ];
	char stkex[SM_STKEX_SZ];

	struct sm_stkui ui;

	struct sm_stock *next;
};

struct sm_desc {
	struct sm_cfg cfg;
	struct sm_stock *stock;
};

struct sm_xmlcfg {
	char *delay_ms;
	char		interval[16];

	struct sm_stock	*stock;
};

#endif
