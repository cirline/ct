#define pr_fmt(fmt)	"market  ] " fmt

#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>
#include "geye/ge.h"
#include "geye/calc.h"
#include "geye/event.h"

enum {
	NC_CODE		= 0,
	NC_PRICE,
	NC_DIFF,
	NC_DIFF_FG,
	NC_ROC,
	NC_NAME,
	NC_MPROC,
	NC_MPROC_FG,
	NC_END,
};

static int g_market_timer_running = 0;

static void market_window_set_active(GtkWidget *widget, int active)
{
	if(active)
		gtk_window_set_keep_above(GTK_WINDOW(widget), TRUE);
#if 0
	else
		gtk_window_set_keep_below(GTK_WINDOW(widget), TRUE);
#endif
}

int market_list_event_cb(GtkWidget *widget, GdkEvent *event, gpointer p)
{
//	pr_info("%s, event type = %d\n", __func__, event->type);
	switch(event->type) {
	case GDK_FOCUS_CHANGE:
		if(gtk_window_is_active(GTK_WINDOW(widget))) {
			gtk_widget_hide(widget);
			market_window_set_active(widget, 0);
		}
		else
			market_window_set_active(widget, 1);
		break;
	case GDK_DELETE:
		g_market_timer_running = 0;
		break;
	}

	return FALSE;
}

void market_list_activate_default_cb(GtkWindow *win, gpointer p)
{
	pr_info("%s\n", __func__);
}

static int market_netdata_update(struct golden_eye_2 *ge)
{
	int rc;

	if(event_netdata_update(ge) < 0) {
		pr_err("netdata update fail\n");
		return - EINVAL;
	}

	calc_realtime_info_v2(ge);
	calc_print_info(ge);

	return 0;
}

static char *market_f2color(float n)
{
	static char *color[] = {
		MARKET_COLOR_RISE,
		MARKET_COLOR_DROP,
		MARKET_COLOR_EQ,
	};

	if(n > 0)
		return color[0];
	else if(n < 0)
		return color[1];
	else
		return color[2];
}

static void market_display_update(struct golden_eye_2 *ge)
{
	struct ge_index *idx;
	struct ge_idxdat *idxd;
	struct ge_stock *stock;
	struct ge_stkdat *stkd;

	GtkTreeIter iter;
	char price_str[16], roc_str[16], diff_str[16], mproc_str[16];
	char *color;

	gtk_list_store_clear(ge->ui.market_index_lstore);

	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;
		sprintf(price_str, "%.2f", idxd->index);
		sprintf(roc_str, "%.2f", idxd->roc);
		sprintf(diff_str, "%.2f", idxd->diff);

		color = market_f2color(idxd->diff);

		gtk_list_store_append(ge->ui.market_index_lstore, &iter);
		gtk_list_store_set(ge->ui.market_index_lstore, &iter,
				1, price_str,
				2, diff_str,
				3, color,
				4, roc_str,
				NC_NAME, idxd->name,
				-1);
	}

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		stkd = &stock->data;

		sprintf(price_str, "%.2f", stkd->price);
		sprintf(roc_str, "%.2f", stock->roc * 100);
		sprintf(diff_str, "%.2f", fabsf(stock->diff));

		color = market_f2color(stock->diff);

		gtk_list_store_append(ge->ui.market_index_lstore, &iter);
		gtk_list_store_set(ge->ui.market_index_lstore, &iter,
				0, stock->code,
				1, price_str,
				2, diff_str,
				3, color,
				4, roc_str,
				NC_NAME, stkd->name,
				-1);
	}
}

static gboolean market_net_request(struct golden_eye_2 *ge)
{
	int rc;
	
	rc = market_netdata_update(ge);
	if(rc < 0)
		return TRUE;

	if(!g_market_timer_running) {
		pr_info("window deleted, stop timer\n");
		return FALSE;
	}

	market_display_update(ge);

	return TRUE;
}

static void market_ui_visual_setup(GtkWidget *win)
{
	gtk_widget_set_app_paintable(win, TRUE);
	GdkScreen *screen = gdk_screen_get_default();
	GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

	if(visual && gdk_screen_is_composited(screen)) {
		gtk_widget_set_visual(win, visual);
	}
}

void market_ui_build(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder = gtk_builder_new_from_file("layout/market_list.glade");

	GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "market_list"));

	ge->ui.market = win;
	ge->ui.market_index_lstore =
		GTK_LIST_STORE(gtk_builder_get_object(builder, "market_index_liststore"));

	gtk_window_set_keep_below(GTK_WINDOW(win), TRUE);

	gtk_application_add_window(app, GTK_WINDOW(win));

	market_ui_visual_setup(win);

	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(win);
}

void market_ui_start(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkWidget *win;

	pr_info("%s\n", __func__);

	market_ui_build(app, ge);

	g_market_timer_running = 1;
	g_timeout_add(20000, (GSourceFunc)market_net_request, ge);

	market_net_request(ge);
}

