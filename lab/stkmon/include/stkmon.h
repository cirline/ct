#ifndef __STKMON_H__
#define __STKMON_H__

#include <gtk/gtk.h>

#define COLOR_RISE	"red"
#define COLOR_DROP	"gray"
#define COLOR_EQ	"black"
#define COLOR_STOPP	"red"
#define COLOR_STOPL	"green"

#define SM_DEFAULT_DELAY_MS	5
#define SM_DEFAULT_INTERVAL	5000

#define SM_CODE_SZ	16
#define SM_STKEX_SZ	8
#define SM_PRICE_SZ	8

struct sm_cfg {
	int delay_ms;
};

struct sm_stkui {
	GtkWidget *label_code;
	GtkWidget *label_price;
	GtkWidget *label_raise;
	GtkWidget	*label_trigger;
	GtkWidget	*toggle_visible;
};

struct sm_stock {
	int	visible;	/* visable on the monitor panel */
	char code[SM_CODE_SZ];
	char stkex[SM_STKEX_SZ];
	char		trigger[SM_PRICE_SZ];
	char		last_minprice[SM_PRICE_SZ];
	char		stop_profit	[SM_PRICE_SZ];
	char		stop_loss	[SM_PRICE_SZ];

	struct sm_stkui ui;
	void		*pull_data;	/* data pull from web */

	struct sm_stock *next;
};

struct sm_desc {
	struct sm_cfg cfg;
	struct sm_stock *stock;
};

struct sm_xmlcfg {
	char		interval[16];

	struct sm_stock	*stock;
	int		stocks_count;
};

#endif
