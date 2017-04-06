#define pr_fmt(fmt)	"ui      ] " fmt

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <ccutils/log.h>

#include "stkmon/stkmon.h"
#include "stkmon/sinajs.h"
#include "geye/ge.h"
#include "geye/event.h"
#include "geye/calc.h"

#define MONITOR_OPACITY		0.4

static int ui_window_set_active_status(GtkWidget *widget, int active)
{
	struct golden_eye_2 *ge;

	ge = g_object_get_data(G_OBJECT(widget), "mdata");
	if(!ge) {
		pr_err("%s cannot get widget data\n", __func__);
		return - ENODATA;
	}

	if(active) {
		gtk_widget_set_visible(ge->ui.monitor_dynamic, TRUE);
		gtk_widget_set_opacity(widget, 1);
		gtk_window_set_decorated(GTK_WINDOW(widget), TRUE);
	} else {
		gtk_widget_set_visible(ge->ui.monitor_dynamic, FALSE);
		gtk_widget_set_opacity(widget, MONITOR_OPACITY);
		gtk_window_resize(GTK_WINDOW(widget), 1, 1);
		gtk_window_set_decorated(GTK_WINDOW(widget), FALSE);
	}

	return 0;
}

int monitor_mwin_event_cb(GtkWidget *widget, GdkEvent *event, gpointer p)
{
	switch(event->type) {
	case GDK_ENTER_NOTIFY:
		ui_window_set_active_status(widget, 1);
		break;
	case GDK_LEAVE_NOTIFY:
		ui_window_set_active_status(widget, 0);
		break;
	case GDK_FOCUS_CHANGE:
		if(gtk_window_is_active(GTK_WINDOW(widget))) {
			ui_window_set_active_status(widget, 0);
		} else {
			ui_window_set_active_status(widget, 1);
		}
		break;
	default:
		pr_debug("unimpl event type = %d\n", event->type);
	}

	return FALSE;
}

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

static gboolean ui_icon_cycle_display(struct golden_eye_2 *ge, float index, float roc)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_text_extents_t extents;
	float ystart;
	char buf[16];
	const int gap = 16;

	int index_dec = (int)(index * 100) % 100;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 64, 64);
	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 0, 0 ,0);
	cairo_set_font_size(cr, 16);

	snprintf(buf, 15, "%.2f", roc);
	cairo_text_extents(cr, buf, &extents);
	ystart = 32 - (gap + extents.height * 3) / 2 + extents.height;
	cairo_move_to(cr, 32 - extents.width / 2, ystart);
	cairo_show_text(cr, buf);

	snprintf(buf, 15, "%d", (int)index % 1000);
	cairo_text_extents(cr, buf, &extents);
	ystart += extents.height + gap / 2;
	cairo_move_to(cr, 32 - extents.width / 2, ystart);
	cairo_show_text(cr, buf);

	snprintf(buf, 15, "%d", index_dec);
	cairo_text_extents(cr, buf, &extents);
	ystart += extents.height + gap / 2;
	cairo_move_to(cr, 32 - extents.width / 2, ystart);
	cairo_show_text(cr, buf);

	cairo_surface_write_to_png(surface, "icon_tmp.png");

	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	gtk_window_set_icon_from_file(GTK_WINDOW(ge->ui.win), "icon_tmp.png", NULL);
}

static void monitor_ui_update(struct golden_eye_2 *ge_)
{
	struct golden_eye *ge = &ge_->old;
	struct ge_index *idx;
	struct stk_stock *stock;
	struct ge_stkdat *dat;
	GdkRGBA color;
	char buffer[16];
	char *pbuf;

	pr_info("%s, in\n", __func__);

	/* updata index ui */
	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {
		struct ge_idxdat *idxd = &idx->data;
#if 0
		gdk_rgba_parse(&color, idxd->roc > 0 ? COLOR_RISE : COLOR_DROP);

		snprintf(buffer, 15, "%.2f", idxd->index - idx->base);
		gtk_label_set_text(GTK_LABEL(idx->ui.index), buffer);
		gtk_widget_override_color(idx->ui.index, GTK_STATE_NORMAL, &color);

		snprintf(buffer, 15, "%.2f", fabsf(idxd->roc));
		gtk_label_set_text(GTK_LABEL(idx->ui.roc), buffer);
		gtk_widget_override_color(idx->ui.roc, GTK_STATE_NORMAL, &color);

		snprintf(buffer, 15, "%.2f", fabsf(idxd->diff));
		gtk_label_set_text(GTK_LABEL(idx->ui.diff), buffer);
		gtk_widget_override_color(idx->ui.diff, GTK_STATE_NORMAL, &color);

		gtk_label_set_text(GTK_LABEL(idx->ui.name), idxd->name);
#endif
		if(asprintf(&pbuf, "%.2f,%.2f,%.2f", idxd->roc, idxd->index, idxd->diff) > 0) {
			gtk_window_set_title(GTK_WINDOW(ge_->ui.win), pbuf);
			free(pbuf);
		}

		//ui_icon_cycle_display(ge_, idxd->index, idxd->roc);
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
		sprintf(buffer, "%.2f", fabsf(stock->chg_rate * 100));
		gtk_label_set_text(GTK_LABEL(stock->ui2.roc), buffer);

		/* roc_lastbuy */
		float stop_profit = stock->cfg.stop_profit.f;
		float stop_loss = stock->cfg.stop_loss.f;
		float roc_lastbuy = stock->chg_rate_min;
		sprintf(buffer, "%.1f", fabsf(roc_lastbuy * 100));
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

		gtk_label_set_text(GTK_LABEL(stock->ui2.name), dat->name);
	}
}

static gboolean monitor_net_request(gpointer p)
{
	int rc;
	struct golden_eye_2 *ge = p;

	rc = monitor_netdata_update(&ge->old);
	if(rc < 0)
		return TRUE;

	monitor_ui_update(ge);
	return TRUE;
}

/*
 * monitor info panel
 * ---------------------
 *  price     roc     roc_lastbuy
 * ---------------------
 */
static GtkWidget *monitor_infopanel_create(GtkBuilder *mbuilder, struct golden_eye_2 *ge2)
{
	GtkBuilder *builder;
	GtkWidget *grid;
	GtkWidget *grid_dynamic;
	struct ge_index *idx;
	struct stk_stock *stock;
	int grid_cur_row = 0;
	struct golden_eye *ge = &ge2->old;

	grid = GTK_WIDGET(gtk_builder_get_object(mbuilder, "monitor_fixed_info"));
	grid_dynamic = GTK_WIDGET(gtk_builder_get_object(mbuilder, "monitor_dynamic_info"));
#if 0
	for(idx = ge->index_list.cqh_first; idx != (void *)&ge->index_list;
			idx = idx->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!idx->visible)
			continue;

		gtk_grid_insert_row(GTK_GRID(grid), grid_cur_row);

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 0, grid_cur_row, 1, 1);
		idx->ui.index = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 1, grid_cur_row, 1, 1);
		idx->ui.roc = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 2, grid_cur_row, 1, 1);
		idx->ui.diff = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid_dynamic), label, 0, grid_cur_row, 1, 1);
		idx->ui.name = label;

		grid_cur_row++;
	}
#endif

	for(stock = ge->stock_list.cqh_first; stock != (void *)&ge->stock_list;
			stock = stock->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!stock->visible)
			continue;

		gtk_grid_insert_row(GTK_GRID(grid), grid_cur_row);

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 0, grid_cur_row, 1, 1);
		stock->ui2.price = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 1, grid_cur_row, 1, 1);
		stock->ui2.roc = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid), label, 2, grid_cur_row, 1, 1);
		stock->ui2.roc_lastbuy = label;

		builder = ge_new_builder_from_file("layout/label/label_right.glade");
		label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
		gtk_grid_attach(GTK_GRID(grid_dynamic), label, 0, grid_cur_row, 1, 1);
		stock->ui2.name = label;

		grid_cur_row++;

	}

	ge2->ui.monitor_dynamic = grid_dynamic;

	return grid;
}

static void monitor_move_window(GtkWidget *win, struct golden_eye_2 *ge2)
{
	int px, py;
	int width, height;
	struct golden_eye *ge = &ge2->old;

	//GdkScreen *screen = gdk_screen_get_resolution();

	gtk_window_get_position(GTK_WINDOW(win), &px, &py);
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pr_info("%d, %d, %d, %d\n", px * 2, py * 2, width, height);
	ge->ui.xpos = px * 2 - width - 160;
	ge->ui.ypos = py * 2 - height;
	ge2->ui.width = width;
	ge2->ui.height = height;
	gtk_window_move(GTK_WINDOW(win), ge->ui.xpos, ge->ui.ypos);
}

static void ui_visual_setup(GtkWidget *widget)
{
	gtk_widget_set_app_paintable(widget, TRUE);

	GdkScreen *screen = gdk_screen_get_default();
	GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

	if(visual && gdk_screen_is_composited(screen))
		gtk_widget_set_visual(widget, visual);
}

void ui_window_start(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder;
	GtkWidget *win;

	pr_info("%s\n", __func__);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "layout/monitor_main.glade", NULL);
	win = GTK_WIDGET(gtk_builder_get_object(builder, "monitor_mwin"));
	gtk_application_add_window(app, GTK_WINDOW(win));

	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

	ui_visual_setup(win);

	ge->ui.win = win;
	g_object_set_data(G_OBJECT(win), "mdata", ge);
	g_object_set_data(G_OBJECT(win), "builder", builder);

	GtkWidget *infopanel = monitor_infopanel_create(builder, ge);

	g_timeout_add(15500, monitor_net_request, ge);

	gtk_builder_connect_signals(builder, NULL);

	g_object_set_data(G_OBJECT(win), "builder", NULL);
	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(win);

	monitor_net_request(ge);
	monitor_move_window(win, ge);
	gtk_widget_set_visible(ge->ui.monitor_dynamic, FALSE);
}
