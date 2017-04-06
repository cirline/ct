#define pr_fmt(fmt)	"market  ] " fmt

#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>
#include "geye/ge.h"
#include "geye/calc.h"
#include "geye/event.h"
#include "geye/common.h"

static GtkStyleContext *gcontext;

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

static void market_widget_clearset_color(GtkWidget *widget, 
		GtkStyleProvider *provider, char *color)
{
	const char *clsa[] = { "index_raise", "index_drop", };
	int i;
	GtkStyleContext *context;

	context = gtk_widget_get_style_context(widget);
	for(i = 0; i < ARRAY_SIZE(clsa); i++) {
		if(gtk_style_context_has_class(context, clsa[i]))
			gtk_style_context_remove_class(context, clsa[i]);
	}

	geye_widget_add_class(widget, provider, color);
}

static void market_display_update_index(struct golden_eye_2 *ge)
{
	struct ge_index *idx;
	struct ge_idxdat *idxd;
	char color[32];
	char str[128];
	const char *tpl = "<span foreground='%s'>%.2f</span>";
	const char *tpl_p = "<span foreground='%s'>%.2f%</span>";

	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {

		idxd = &idx->data;

		geye_roc_to_colorname(color, idxd->roc, "#bbb");

		sprintf(str, tpl, color, idxd->index);
		gtk_label_set_markup(GTK_LABEL(ge->ui.market.sh.price), str);

		sprintf(str, tpl_p, color, idxd->roc);
		gtk_label_set_markup(GTK_LABEL(ge->ui.market.sh.roc), str);

		sprintf(str, tpl, color, idxd->diff);
		gtk_label_set_markup(GTK_LABEL(ge->ui.market.sh.diff), str);
	}
}

static void market_display_update(struct golden_eye_2 *ge)
{
	struct ge_stock *stock;
	struct ge_stkdat *stkd;

	GtkTreeIter iter;
	char price_str[16], roc_str[16], diff_str[16], mproc_str[16];
	char color[16];

	market_display_update_index(ge);

	gtk_list_store_clear(GTK_LIST_STORE(ge->ui.market.lstore));

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		stkd = &stock->data;

		sprintf(price_str, "%.2f", stkd->price);
		sprintf(roc_str, "%.2f", fabsf(stock->roc * 100));
		sprintf(diff_str, "%.2f", fabsf(stock->diff));

		geye_roc_to_colorname(color, stock->roc, "#bbb");

		gtk_list_store_append(GTK_LIST_STORE(ge->ui.market.lstore), &iter);
		gtk_list_store_set(GTK_LIST_STORE(ge->ui.market.lstore), &iter,
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

	/* load css */
	GtkStyleContext *context = gtk_widget_get_style_context(win);
	GtkStyleProvider *provider = (GtkStyleProvider *)gtk_css_provider_new();
	gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "res/styles.css", NULL);
	gtk_style_context_add_provider(context, provider,
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	ge->ui.market.win = win;
	ge->ui.market.context = context;
	ge->ui.market.provider = provider;

	ge->ui.market.lstore =
		GTK_WIDGET(gtk_builder_get_object(builder, "market_index_liststore"));
	ge->ui.market.sh.price =
		GTK_WIDGET(gtk_builder_get_object(builder, "market_index_sh_price"));
	ge->ui.market.sh.roc =
		GTK_WIDGET(gtk_builder_get_object(builder, "market_index_sh_roc"));
	ge->ui.market.sh.diff =
		GTK_WIDGET(gtk_builder_get_object(builder, "market_index_sh_diff"));
	gtk_window_set_keep_below(GTK_WINDOW(win), TRUE);
	gtk_application_add_window(app, GTK_WINDOW(win));


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

