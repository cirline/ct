#define pr_fmt(fmt)	"market  ] " fmt

#include <errno.h>
#include <math.h>
#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>
#include "geye/ge.h"
#include "geye/calc.h"
#include "geye/event.h"

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
		/* pr_info("%16s %8.2f %8.3f %12.2f\n",
				idxd->code, idxd->index, idxd->roc, idxd->diff); */
		sprintf(price_str, "%.2f", idxd->index);
		sprintf(roc_str, "%.2f", idxd->roc);
		sprintf(diff_str, "%.2f", idxd->diff);

		color = market_f2color(idxd->diff);

		gtk_list_store_append(ge->ui.market_index_lstore, &iter);
		gtk_list_store_set(ge->ui.market_index_lstore, &iter,
				0, idxd->code,
				1, price_str,
				2, diff_str,
				3, color,
				4, roc_str,
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
				-1);
	}
}

static gboolean market_net_request(struct golden_eye_2 *ge)
{
	int rc;
	
	rc = market_netdata_update(ge);
	if(rc < 0)
		return TRUE;

	market_display_update(ge);

	return TRUE;
}

void market_ui_build(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder = gtk_builder_new_from_file("layout/market_list.glade");

	GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "market_list"));
//	GtkTreeView *tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "market_index_treeview"));
	ge->ui.market_index_lstore = GTK_LIST_STORE(gtk_builder_get_object(builder, "market_index_liststore"));

#if 0
#endif

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

	//ge->ui.market = win;
	//g_object_set_data(G_OBJECT(win), "mdata", ge);
	//g_object_set_data(G_OBJECT(win), "builder", builder);

	g_timeout_add(5500, (GSourceFunc)market_net_request, ge);

	//g_object_set_data(G_OBJECT(win), "builder", NULL);

	market_net_request(ge);
}

