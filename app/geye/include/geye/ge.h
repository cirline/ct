#ifndef __GOLDEN_EYE_GE_H__
#define __GOLDEN_EYE_GE_H__

#include <gtk/gtk.h>
#include "geye/ui.h"
#include "stkmon/stkmon.h"

struct ge_stkcfg {
	int	enable;
	int	visible;

	float	min_price;
	float	aim_price;
	float	avg_price;
	float	stop_profit;
	float	stop_loss;
};

#if 0
struct ge_stkui {
	GtkWidget	*widget;
};
#endif

#define GE_ABCODE_SZ	8
struct ge_stock {
	struct ge_stkdat	data;

	char	code[STK_CODE_SZ];
	char	exchange[STK_EX_SZ];
	char	abcode[GE_ABCODE_SZ];
	int	num;
	float	diff;
	float	roc;
	float	mproc;		/* roc for min price */

//	struct ge_stkui		ui;
	struct ge_stkcfg	cfg;

	CIRCLEQ_ENTRY(ge_stock)	list;
};

struct ge_stkdat_container {
	struct ge_stkdat	common;
};
#define GE_GET_STKDAT(pull_data) \
	(&((struct ge_stkdat_container *)(pull_data))->common)

struct golden_eye_2 {
	char		version[32];
	struct ge_ui_main	ui;

	struct golden_eye	old;

	int (*pull_stock_data)(struct golden_eye_2 *ge);
	int (*pull_index_data)(struct golden_eye_2 *ge);

	int			stock_count;
	CIRCLEQ_HEAD(, ge_stock)
				stock_list;

	int			index_count;
	CIRCLEQ_HEAD(, ge_index)
				index_list;
};

#endif
