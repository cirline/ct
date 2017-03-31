#ifndef __GOLDEN_EYE_UI_H__
#define __GOLDEN_EYE_UI_H__

#include "gtk/gtk.h"

struct ge_ui_index {
	GtkWidget	*price, *roc, *diff;
};

struct ge_ui_market {
	GtkStyleContext		*context;
	GtkStyleProvider	*provider;
	GtkWidget		*win;
	GtkWidget		*lstore;
	struct ge_ui_index	sh, sz, cy;
};

struct ge_ui_main {
	struct ge_ui_market	market;

	/* monitor */
	GtkWidget	*monitor;
	GtkListStore	*monitor_lstore;
	GtkTreeViewColumn	*monitor_cell_roc;
	GtkTreeViewColumn	*monitor_cell_name;
};

#endif
