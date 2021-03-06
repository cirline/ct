#define pr_fmt(fmt)	"monitor ] " fmt

#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>
#include "geye/ge.h"

enum {
	NC_PRICE	= 0,
	NC_TIP,
	NC_TIP_BG,
	NC_ROC,
	NC_NAME,
	NC_TIP_FG,
};

static int g_monitor_timer_running = 0;

static int monitor_set_active_status(GtkWidget *widget, int active)
{
	struct golden_eye_2 *ge;

	ge = g_object_get_data(G_OBJECT(widget), "ge");
	if(!ge) {
		pr_err("%s, can't get widget data\n", __func__);
		return - ENODATA;
	}

	GtkWidget *win = ge->ui.monitor;

	if(active) {
		gtk_window_set_decorated(GTK_WINDOW(widget), TRUE);

		gtk_tree_view_column_set_visible(
				GTK_TREE_VIEW_COLUMN(ge->ui.monitor_cell_roc), TRUE);
		gtk_tree_view_column_set_visible(
				GTK_TREE_VIEW_COLUMN(ge->ui.monitor_cell_name), TRUE);
	} else {
		gtk_window_set_decorated(GTK_WINDOW(widget), FALSE);

		gtk_tree_view_column_set_visible(
				GTK_TREE_VIEW_COLUMN(ge->ui.monitor_cell_roc), FALSE);
		gtk_tree_view_column_set_visible(
				GTK_TREE_VIEW_COLUMN(ge->ui.monitor_cell_name), FALSE);

		gtk_window_resize(GTK_WINDOW(widget), 1, 1);
	}
	return 0;
}

static int monitor_show_market(GtkWidget *widget, int active)
{
	struct golden_eye_2 *ge;

	ge = g_object_get_data(G_OBJECT(widget), "ge");
	if(!ge) {
		pr_err("%s, can't get widget data\n", __func__);
		return - ENODATA;
	}

	pr_here();

	if(active)
		gtk_widget_show_all(ge->ui.market.win);
	else
		gtk_widget_hide(ge->ui.market.win);
}

static int monitor_event_to_string(int type)
{
	switch(type) {
	case GDK_DELETE:
		pr_info("event ~ GDK_DELETE\n");
		break;
	case GDK_ENTER_NOTIFY:
		pr_info("event ~ GDK_ENTER_NOTIFY\n");
		break;
	case GDK_LEAVE_NOTIFY:
		pr_info("event ~ GDK_LEAVE_NOTIFY\n");
		break;
	case GDK_FOCUS_CHANGE:
		pr_info("event ~ GDK_FOCUS_CHANGE\n");
		break;
	case GDK_MOTION_NOTIFY:
//		pr_info("event ~ GDK_MOTION_NOTIFY\n");
		break;
	case GDK_BUTTON_PRESS:
		pr_info("event ~ GDK_BUTTON_PRESS\n");
		break;
	case GDK_BUTTON_RELEASE:
		pr_info("event ~ GDK_BUTTON_RELEASE\n");
		break;
	case GDK_WINDOW_STATE:
		pr_info("event ~ GDK_WINDOW_STATE\n");
		break;
	default:
		pr_info("event ~ %d\n", type);
	}
}

static void monitor_delete(GtkWidget *widget)
{
	struct golden_eye_2 *ge;

	ge = g_object_get_data(G_OBJECT(widget), "ge");
	if(!ge) {
		pr_err("%s, can't get widget data\n", __func__);
		return;
	}

	gtk_widget_destroy(ge->ui.market.win);
}

int monitor_event_cb(GtkWidget *widget, GdkEvent *event, gpointer p)
{
	//monitor_event_to_string(event->type);
	switch(event->type) {
	case GDK_DELETE:
		g_monitor_timer_running = 0;
		monitor_delete(widget);
		break;
	case GDK_ENTER_NOTIFY:
		//pr_info("state = %x\n", event->crossing.state);
		monitor_set_active_status(widget, 1);
		break;
	case GDK_LEAVE_NOTIFY:
		/* if window is active, we can hide the cols at `focus-change`
		 * but, if window not active, we should hide the cols now */
		if(!gtk_window_is_active(GTK_WINDOW(widget)))
			monitor_set_active_status(widget, 0);
		break;
	case GDK_FOCUS_CHANGE:
		if(gtk_window_is_active(GTK_WINDOW(widget))) {
			monitor_set_active_status(widget, 0);
		}
		break;
	case GDK_KEY_PRESS:
		if(event->key.keyval == GDK_KEY_Tab)
			monitor_show_market(widget, 1);
		break;
	}
	return FALSE;
}

static int monitor_f2color(char *ob, float roc)
{
	int m, n;
	int roc_1000;

	roc_1000 = fabsf(roc * 10);
	if(roc_1000 > 99)
		roc = 99;

	m = 9 - roc_1000 / 10;
	n = 9 - roc_1000 % 10;
//	pr_info("roc %f, roc_1000 %d, m %d, n %d\n", roc, roc_1000, m, n);

	if(roc > 0)
		sprintf(ob, "#FF%x%x%x%x", m, n, m, n);
	else if(roc < 0)
		sprintf(ob, "#%x%x%x%xFF", m, n, m, n);
	else
		strcpy(ob, "#bbb");

	return roc_1000;
}

static void monitor_display_update(struct golden_eye_2 *ge)
{
	struct ge_index *idx;
	struct ge_idxdat *idxd;
	struct ge_stock *stock;
	struct ge_stkdat *stkd;
	GtkTreeIter iter;
	char price_str[16], roc_str[16], diff_str[16], mproc_str[16], tip_str[16];
	char color[16];
	int rc;

	pr_here();

	gtk_list_store_clear(ge->ui.monitor_lstore);

	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;
		rc = monitor_f2color(color, idxd->roc);

		sprintf(price_str, "%.2f", idxd->index);
		sprintf(tip_str, "%1d", abs((int)(idxd->roc)));
		sprintf(diff_str, "%.2f", idxd->diff);
		sprintf(roc_str, "%.2f", fabsf(idxd->roc));

		//pr_info("roc %s, color = %s\n", roc_str, color);
		gtk_list_store_append(ge->ui.monitor_lstore, &iter);
		gtk_list_store_set(ge->ui.monitor_lstore, &iter,
				0, price_str,
				1, tip_str,
				2, color,
				3, roc_str,
				4, idxd->name,
				-1);
	}

	/* stock onboard */
	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		if(stock->num <= 0)
			continue;

		stkd = &stock->data;

		sprintf(price_str, "%.2f", stkd->price);
		sprintf(tip_str, "%1d", abs((int)(stock->roc * 100)));
		sprintf(diff_str, "%.2f", fabsf(stock->diff));
		sprintf(roc_str, "%.2f", fabsf(stock->roc) * 100);

		monitor_f2color(color, stock->roc * 100);

		//pr_info("roc %s, color = %s\n", roc_str, color);
		gtk_list_store_append(ge->ui.monitor_lstore, &iter);
		gtk_list_store_set(ge->ui.monitor_lstore, &iter,
				0, price_str,
				1, tip_str,
				2, color,
				3, roc_str,
				4, stkd->name,
				-1);

		//g_object_set(cell_roc, "cell-background", color, NULL);
	}

	/* cut-off line */
	gtk_list_store_append(ge->ui.monitor_lstore, &iter);
	gtk_list_store_set(ge->ui.monitor_lstore, &iter, 2, "#000", -1);

	/* stock wait and see */
	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		if(stock->num > 0)
			continue;

		stkd = &stock->data;

		sprintf(price_str, "%.2f", stkd->price);
		sprintf(tip_str, "%1d", abs((int)(stock->roc * 100)));
		sprintf(diff_str, "%.2f", fabsf(stock->diff));
		sprintf(roc_str, "%.2f", fabsf(stock->roc) * 100);

		monitor_f2color(color, stock->roc * 100);

		//pr_info("roc %s, color = %s\n", roc_str, color);
		gtk_list_store_append(ge->ui.monitor_lstore, &iter);
		gtk_list_store_set(ge->ui.monitor_lstore, &iter,
				0, price_str,
				1, tip_str,
				2, color,
				3, roc_str,
				4, stkd->name,
				-1);

		//g_object_set(cell_roc, "cell-background", color, NULL);
	}

	gtk_window_resize(GTK_WINDOW(ge->ui.monitor), 1, 1);
}

static gboolean monitor_net_request(struct golden_eye_2 *ge)
{
	if(!g_monitor_timer_running) {
		pr_info("window deleted, stop timer\n");
		return FALSE;
	}

	monitor_display_update(ge);

	return TRUE;
}

static GtkWidget *monitor_ui_build(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder = gtk_builder_new_from_file("layout/monitor_list.glade");

	GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "monitor"));
	ge->ui.monitor = win;
	ge->ui.monitor_lstore =
		GTK_LIST_STORE(gtk_builder_get_object(builder, "monitor_liststore"));
	ge->ui.monitor_cell_roc =
		GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col_roc"));
	ge->ui.monitor_cell_name =
		GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "col_name"));

	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

	gtk_application_add_window(app, GTK_WINDOW(win));

	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(win);

	GdkScreen *screen = gdk_screen_get_default();
	int width = gdk_screen_get_width(screen);
	int height = gdk_screen_get_height(screen);

	gtk_window_move(GTK_WINDOW(win), width - 100, height / 2);

	return win;
}

void monitor_ui_start(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkWidget *win;

	pr_info("%s\n", __func__);
	win = monitor_ui_build(app, ge);

	g_object_set_data(G_OBJECT(win), "ge", ge);

	g_monitor_timer_running = 1;

	g_timeout_add(30000, (GSourceFunc)monitor_net_request, ge);

	monitor_net_request(ge);
}
