#ifndef __GOLDEN_EYE_GEHEAD_H__
#define __GOLDEN_EYE_GEHEAD_H__

#include <gtk/gtk.h>

struct ge_stkui {
	GtkWidget	*price;
	GtkWidget	*roc;
	GtkWidget	*roc_lastbuy;
};

struct ge_idxui {
	GtkWidget	*name;
	GtkWidget	*index;
	GtkWidget	*diff;
	GtkWidget	*roc;
};

#endif
