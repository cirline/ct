/**
 * Golden Eye
 */
#ifndef __STKMON_H__
#define __STKMON_H__

#include <sys/queue.h>
#include <gtk/gtk.h>

#include "geye/ge.h"

#define COLOR_RISE_1	"#FF5555"

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
#define GE_CODE_SIZE	16
#define STK_CODE_SZ	GE_CODE_SIZE
#define STK_NAME_SZ	32


#ifdef DATADIR
#define top_datadir	DATADIR"/"
#else
#define top_datadir	"./"
#endif

struct stk_float {
	char	c	[STK_FLOAT_SZ];
	float	f;
};

struct stk_long {
	char	c	[STK_LONG_SZ];
	long	l;
};

#define ge_stkdat	stk_stkdat
struct stk_stkdat {
	char	exchange[STK_EX_SZ];
	char	code[STK_CODE_SZ];
	char	name[STK_NAME_SZ];
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

/**
 * stock index common data
 * a customized stk_idxdat_xxx should contain it first by named 'common'
 * a customized stk_idxdat_xxx can get the common data by macro STK_GET_IDXDAT()
 */
#define ge_idxdat	stk_idxdat
struct stk_idxdat {
	char	code[STK_CODE_SZ];
	char	name[STK_NAME_SZ];
	float	index;
	float	index_diff;		/* index(today) - index(yesterday) */
	float	roc;			/* rate of change */
	long	volume;
	long	amount;
};
struct stk_idxdat_container {
	struct stk_idxdat	common;
};
#define STK_GET_IDXDAT(pull_data) \
	(&((struct stk_idxdat_container *)(pull_data))->common)

struct sm_stkui {
	GtkWidget *label_code;
	GtkWidget *label_price;
	GtkWidget *label_raise;
	GtkWidget	*label_trigger;
	GtkWidget	*toggle_visible;
	GtkWidget	*label_avg_price;
	GtkWidget	*label_name;
};

struct stk_idxui {
	GtkWidget	*label_index;
	GtkWidget	*label_roc;
	GtkWidget	*label_name;

	GtkWidget	*index;
	GtkWidget	*roc;
	GtkWidget	*name;
};

struct stk_stkcfg {
	struct stk_float	min_price;
	struct stk_float	aim_price;
	struct stk_float	avg_price;
	struct stk_float	stop_profit;
	struct stk_float	stop_loss;
};

#define ge_stock	stk_stock
struct stk_stock {
	int	visible;	/* visable on the monitor panel */
	char code[SM_CODE_SZ];
	char	exchange[STK_EX_SZ];

	struct stk_stkcfg	cfg;

	struct sm_stkui ui;
	struct ge_stkui		ui2;
	void		*pull_data;	/* data pull from web, link to stk_stkdat_container */
	float	chg_rate;
	float	chg_rate_min;

	int	chg_rate_alert;

	CIRCLEQ_ENTRY(stk_stock)	list;
};
#define sm_stock	stk_stock

#define ge_index	stk_index
struct stk_index {
	struct stk_idxdat	data;
	struct stk_idxui	ui;

	short			enable;
	short			visible;
	int			base;

	CIRCLEQ_ENTRY(stk_index)
				list;
};

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

struct stk_mainui {
	int	xpos;
	int	ypos;
	int	width;
	int	height;

	GtkWidget	*win;
	GtkWidget	*menu;
	struct {
		GtkWidget	*fixed;
		GtkWidget	*dynamic;
	} monitor;
};

struct golden_eye {
	char		interval[16];
	struct stk_alert	alert;

	struct stk_mainui	ui;


	int (*pull_index_data)(struct golden_eye *ge);

	CIRCLEQ_HEAD(, stk_stock)	
				stock_list;
	int		stocks_count;

	CIRCLEQ_HEAD(, stk_index)
				index_list;
	int			index_count;
};
#define sstkmon		golden_eye
#define stk_xmlcfg	sstkmon
#define sm_xmlcfg	stk_xmlcfg

#endif
