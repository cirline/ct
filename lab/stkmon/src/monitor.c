#define pr_fmt(fmt)	"monitor ] " fmt

#include <errno.h>
#include <gtk/gtk.h>
#include <ccutils/log.h>

#include "stkmon/stkmon.h"
#include "stkmon/sinajs.h"

static GtkBuilder *ge_new_builder_from_file(char *fn)
{
	GtkBuilder *builder;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, fn, NULL);

	return builder;
}

static int monitor_netdata_update(struct golden_eye *ge)
{
	int rc;

	if(event_update_net_data(ge) < 0)
		return - EINVAL;

	rc = sinajs_pull_data(ge);
	if(rc < 0) {
		pr_err("sinajs_pull_data failed\n");
		return - EINVAL;
	}

	sinajs_apply_data(ge);
	calc_realtime_info(ge);
	calc_pr_info(ge);

	return 0;
}

static void monitor_ui_update(struct golden_eye *ge)
{
	struct ge_index *idx;
	struct ge_stock *stock;
	struct ge_stkdat *dat;
	GdkRGBA color;
	char buffer[16];

	/* updata index ui */
	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {
		struct ge_idxdat *idxd = &idx->data;

		snprintf(buffer, 15, "%.2f", idxd->index - idx->base);
		gtk_label_set_text(GTK_LABEL(idx->ui.index), buffer);

		snprintf(buffer, 15, "%.2f", idxd->roc);
		gtk_label_set_text(GTK_LABEL(idx->ui.roc), buffer);

		//gtk_label_set_text(GTK_LABEL(idx->ui.name), idxd->name);
	}

	/* update stock ui */
	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		if(!stock->pull_data)
			continue;

		dat = STK_GET_STKDAT(stock->pull_data);

		/* price */
		sprintf(buffer, "%.2f", dat->price);
		gtk_label_set_text(GTK_LABEL(stock->ui2.price), buffer);

		/* roc */
		sprintf(buffer, "%.2f", stock->chg_rate * 100);
		gtk_label_set_text(GTK_LABEL(stock->ui2.roc), buffer);

		/* roc_lastbuy */
		float stop_profit = stock->cfg.stop_profit.f;
		float stop_loss = stock->cfg.stop_loss.f;
		float roc_lastbuy = stock->chg_rate_min;
		sprintf(buffer, "%.1f", roc_lastbuy * 100);
		gtk_label_set_text(GTK_LABEL(stock->ui2.roc_lastbuy), buffer);

		if(roc_lastbuy >= stop_profit - 1)
			gdk_rgba_parse(&color, COLOR_STOPP);
		else if(roc_lastbuy <= stop_loss - 1)
			gdk_rgba_parse(&color, COLOR_STOPL);
		else
			gdk_rgba_parse(&color, COLOR_EQ);
		gtk_widget_override_color(stock->ui2.roc_lastbuy, GTK_STATE_NORMAL, &color);

		if(dat->price > dat->pre_close)
			gdk_rgba_parse(&color, COLOR_RISE);
		else if(dat->price < dat->pre_close)
			gdk_rgba_parse(&color, COLOR_DROP);
		else
			gdk_rgba_parse(&color, COLOR_EQ);
		gtk_widget_override_color(stock->ui2.price, GTK_STATE_NORMAL, &color);
		gtk_widget_override_color(stock->ui2.roc, GTK_STATE_NORMAL, &color);

		//gtk_label_set_text(GTK_LABEL(stock->ui.label_name), dat->name);
	}
}

static gboolean monitor_net_request(gpointer p)
{
	int rc;

	rc = monitor_netdata_update(p);
	if(rc < 0)
		return TRUE;

	monitor_ui_update(p);
	return TRUE;

}

/*
 * monitor info panel
 * ---------------------
 *  price     roc     roc_lastbuy
 * ---------------------
 */
static GtkWidget *monitor_infopanel_create(GtkBuilder *mbuilder, struct golden_eye *ge)
{
	GtkBuilder *builder;
	GtkWidget *grid;
	struct ge_index *idx;
	struct ge_stock *stock;
	int grid_cur_row = 0;

	grid = GTK_WIDGET(gtk_builder_get_object(mbuilder, "monitor_grid"));

	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!idx->visible)
			continue;

		gtk_grid_insert_row(GTK_GRID(grid), grid_cur_row);

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), align, 0, grid_cur_row, 1, 1);
		idx->ui.index = label;

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), align, 1, grid_cur_row, 1, 1);
		idx->ui.roc = label;

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		gtk_grid_attach(GTK_GRID(grid), align, 2, grid_cur_row, 1, 1);

		grid_cur_row++;
	}

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!stock->visible)
			continue;

		gtk_grid_insert_row(GTK_GRID(grid), grid_cur_row);

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), align, 0, grid_cur_row, 1, 1);
		stock->ui2.price = label;

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), align, 1, grid_cur_row, 1, 1);
		stock->ui2.roc = label;

		builder = ge_new_builder_from_file("layout/label/rightlabel.glade");
		align = GTK_WIDGET(gtk_builder_get_object(builder, "align"));
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), align, 2, grid_cur_row, 1, 1);
		stock->ui2.roc_lastbuy = label;

		grid_cur_row++;

	}

	return grid;
}

static monitor_move_window(GtkWidget *win, struct golden_eye *ge)
{
	int px, py;
	int width, height;

	//GdkScreen *screen = gdk_screen_get_resolution();

	gtk_window_get_position(GTK_WINDOW(win), &px, &py);
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pr_info("%d, %d, %d, %d\n", px * 2, py * 2, width, height);
	ge->ui.xpos = px * 2 - width - 160;
	ge->ui.ypos = py * 2 - height;
	ge->ui.width = width;
	ge->ui.height = height;
	gtk_window_move(GTK_WINDOW(win), ge->ui.xpos, ge->ui.ypos);
}

void monitor_main_window(int argc, char *argv[], struct golden_eye *ge)
{
	GtkBuilder *builder;
	GtkWidget *win;

	pr_info("%s\n", __func__);

	gtk_init(&argc, &argv);
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "layout/monitor_main.glade", NULL);
	win = GTK_WIDGET(gtk_builder_get_object(builder, "monitor_mwin"));
	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

	GtkWidget *infopanel = monitor_infopanel_create(builder, ge);

	g_timeout_add(10000, monitor_net_request, ge);
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(G_OBJECT(builder));
	gtk_widget_show_all(win);
	monitor_net_request(ge);
	monitor_move_window(win, ge);

	gtk_main();
}

