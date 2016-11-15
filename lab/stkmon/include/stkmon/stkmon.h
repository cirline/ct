/**
 * sm - stock market
 */
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
#define STK_FLOAT_SZ	8
#define STK_LONG_SZ	16
#define STK_DATA_SZ	16
#define STK_TIME_SZ	16
#define STK_EX_SZ	8
#define STK_CODE_SZ	8

struct sm_price {
	char	c	[SM_PRICE_SZ];
	float	f;
};

struct stk_float {
	char	c	[STK_FLOAT_SZ];
	float	f;
};

struct stk_long {
	char	c	[STK_LONG_SZ];
	long	l;
};

struct stk_stkdat {
	char	exchange[STK_EX_SZ];
	char	code[STK_CODE_SZ];
	float	open;
	float	pre_close;
	float	price;
	float	high, low;
	float	bid, ask;
	long	volume;
	long	amount;
	int	bv[5];		/* bid volume */
	float	vp[5];		/* bid price */
	int	av[5];		/* ask volume */
	float	ap[5];		/* ask price */
	char	date[STK_DATA_SZ];
	char	time[STK_TIME_SZ];
};

struct stk_stkdat_container {
	struct stk_stkdat	common;
};
#define STK_GET_STKDAT(pull_data) \
	(&((struct stk_stkdat_container *)(pull_data))->common)

struct stk_idxdat {
	char	exchange[STK_EX_SZ];
	char	code[STK_CODE_SZ];
	float	index;
	float	volume;
	float	amount;
};

struct stk_idxdat_container {
	struct stk_idxdat	common;
};

struct sm_stkui {
	GtkWidget *label_code;
	GtkWidget *label_price;
	GtkWidget *label_raise;
	GtkWidget	*label_trigger;
	GtkWidget	*toggle_visible;
	GtkWidget	*label_avg_price;
};

struct stk_stkcfg {
	struct stk_float	min_price;
};

struct sm_stock {
	int	visible;	/* visable on the monitor panel */
	char code[SM_CODE_SZ];
	char stkex[SM_STKEX_SZ];
	char		trigger[SM_PRICE_SZ];
	char		last_minprice[SM_PRICE_SZ];
	char		stop_profit	[SM_PRICE_SZ];
	char		stop_loss	[SM_PRICE_SZ];

	struct stk_stkcfg	cfg;

	struct sm_price	avg_price;

	struct sm_stkui ui;
	void		*pull_data;	/* data pull from web */
	float	chg_rate;
	float	chg_rate_min;

	struct sm_stock *next;
};
#define stk_stock	sm_stock

struct stk_alert_level {
	struct stk_float	lv1;
	struct stk_float	lv2;
	struct stk_float	lv3;
};

struct stk_alert {
	struct stk_alert_level	short_term;
	struct stk_alert_level	medium_term;
	struct stk_alert_level	long_term;
};

struct sm_xmlcfg {
	char		interval[16];
	struct stk_alert	alert;

	struct sm_stock	*stock;
	int		stocks_count;
};
#define stk_xmlcfg	sm_xmlcfg

#endif
