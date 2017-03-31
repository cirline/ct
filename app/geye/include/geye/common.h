#ifndef __GOLDEN_EYE_COMMON_H_
#define __GOLDEN_EYE_COMMON_H_

#include <gtk/gtk.h>

int geye_float2color(char *outbuf, float roc_100, char *defval);
void geye_label_set_text(GtkWidget *label, char *str, char *fg,
		GtkStyleContext *context);
void geye_widget_add_class(GtkWidget *widget, GtkStyleProvider *provider, char *class);

#endif
