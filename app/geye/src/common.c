#include <stdio.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>

/**
 * geye_roc_to_color - covert a roc% to color
 * @outbuf: color output buffer
 * @roc_100: the roc * 100
 * @defval: the default value when roc = 0
 *
 * function return the abs(roc * 1000)
 */
int geye_roc_to_colorname(char *outbuf, float roc_100, char *defval)
{
	int m, n;
	int roc_1000;	/* it's roc * 1000 */

	roc_1000 = fabsf(roc_100 * 10);
	if(roc_1000 > 99)
		roc_1000 = 99;

	m = 9 - roc_1000 / 10;
	n = 9 - roc_1000 % 10;

	if(roc_100 > 0)
		sprintf(outbuf, "#FF%x%x%x%x", m, n, m, n);
	else if(roc_100 < 0)
		sprintf(outbuf, "#%x%x%x%xFF", m, n, m, n);
	else
		strcpy(outbuf, defval);

	return roc_1000;
}

void geye_widget_add_class(GtkWidget *widget, GtkStyleProvider *provider, char *class)
{
	GtkStyleContext *context;

	context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_provider(context, provider,
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_style_context_add_class(context, class);
}

