#define pr_fmt(fmt)	"monitor ] " fmt

#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>
#include "geye/ge.h"

static int g_monitor_timer_running = 0;

int monitor_event_cb(GtkWidget *widget, GdkEvent *event, gpointer p)
{
//	pr_info("%s, event type = %d\n", __func__, event->type);
	switch(event->type) {
	case GDK_DELETE:
		g_monitor_timer_running = 0;
		break;
	}

	return FALSE;
}

static char *monitor_f2color(char *ob, float roc)
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
		strcpy(ob, "#000000");

	return ob;
}

static void monitor_display_update(struct golden_eye_2 *ge)
{
	struct ge_index *idx;
	struct ge_idxdat *idxd;
	struct ge_stock *stock;
	struct ge_stkdat *stkd;

	GtkTreeIter iter;
	char price_str[16], roc_str[16], diff_str[16], mproc_str[16];
	char color[16];

	pr_here();

	gtk_list_store_clear(ge->ui.monitor_lstore);

	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;
		sprintf(price_str, "%.2f", idxd->index);
		sprintf(roc_str, "%.2f", idxd->roc);
		sprintf(diff_str, "%.2f", idxd->diff);

		monitor_f2color(color, idxd->roc);

		pr_info("roc %s, color = %s\n", roc_str, color);
		gtk_list_store_append(ge->ui.monitor_lstore, &iter);
		gtk_list_store_set(ge->ui.monitor_lstore, &iter,
				0, price_str,
				1, roc_str,
				2, color,
				-1);
	}

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		stkd = &stock->data;

		sprintf(price_str, "%.2f", stkd->price);
		sprintf(roc_str, "%.2f", stock->roc * 100);
		sprintf(diff_str, "%.2f", fabsf(stock->diff));

		monitor_f2color(color, stock->roc * 100);

		pr_info("roc %s, color = %s\n", roc_str, color);
		gtk_list_store_append(ge->ui.monitor_lstore, &iter);
		gtk_list_store_set(ge->ui.monitor_lstore, &iter,
				0, price_str,
				1, roc_str,
				2, color,
				-1);

		//g_object_set(cell_roc, "cell-background", color, NULL);
	}
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

static void monitor_ui_build(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder = gtk_builder_new_from_file("layout/monitor_list.glade");

	GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "monitor"));
	ge->ui.monitor_lstore =
		GTK_LIST_STORE(gtk_builder_get_object(builder, "monitor_liststore"));

	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

	gtk_application_add_window(app, GTK_WINDOW(win));

	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(win);

	GdkScreen *screen = gdk_screen_get_default();
	int width = gdk_screen_get_width(screen);
	int height = gdk_screen_get_height(screen);

	gtk_window_move(GTK_WINDOW(win), width - 100, height / 2);
}

void monitor_ui_start(GtkApplication *app, struct golden_eye_2 *ge)
{
	pr_info("%s\n", __func__);

	monitor_ui_build(app, ge);

	g_monitor_timer_running = 1;

	g_timeout_add(10000, (GSourceFunc)monitor_net_request, ge);

	monitor_net_request(ge);
}
