#define pr_fmt(fmt)	"stkmon  ] " fmt

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <ccutils/net.h>
#include <ccutils/log.h>

#include "stkmon/ui.h"
#include "stkmon/sinajs.h"
#include "stkmon/stkmon.h"
#include "stkmon/stkxml.h"
#include "stkmon/calc.h"
#include "config.h"

#define WIN_OPACITY	0.6

static int gx = 99;

gboolean hdlr_1s(gpointer *);
void configure_main(GtkWidget *widget, gpointer p);

GtkWidget *create_menubar(GtkWidget *win)
{
	GtkAccelGroup *accel_grp = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(win), accel_grp);

	GtkWidget *menubar = gtk_menu_bar_new();
	GtkWidget *file_item = gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);

	// file menu
	GtkWidget *file_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);

	GtkWidget *quit_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_grp);
	gtk_widget_add_accelerator(quit_item, "activate", accel_grp, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(quit_item), "activate", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *item_config = gtk_menu_item_new_with_label("configure");
	g_signal_connect(G_OBJECT(item_config), "activate", G_CALLBACK(configure_main), &gx);

	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), item_config);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit_item);

	return menubar;
}

int show_popup(GtkWidget *widget, GdkEvent *event)
{
	if(event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *button_event = (GdkEventButton *)event;

		if(button_event->button == 3) {
			gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL, button_event->button, button_event->time);

		}
		return TRUE;
	}

	return FALSE;
}

static int main_event(GtkWidget *widget, GdkEvent *event)
{
	gboolean focus;

	switch(event->type) {
	case GDK_ENTER_NOTIFY:
		gtk_window_set_opacity(GTK_WINDOW(widget), 1);
		break;
	case GDK_LEAVE_NOTIFY:
		gtk_window_set_opacity(GTK_WINDOW(widget), WIN_OPACITY);
		break;
	case GDK_FOCUS_CHANGE:
		if(gtk_window_is_active(GTK_WINDOW(widget)))
			gtk_window_set_opacity(GTK_WINDOW(widget), WIN_OPACITY);
		else
			gtk_window_set_opacity(GTK_WINDOW(widget), 1);
		break;
	default:
		pr_info("unimpl event type = %d\n", event->type);
	}


	return FALSE;
}

void create_popupmenu(GtkWidget *ebox)
{
	GtkWidget *item;

	GtkWidget *popup_menu = gtk_menu_new();

	GtkWidget *item_config = gtk_menu_item_new_with_label("configure");
	gtk_widget_show(item_config);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item_config);
	g_signal_connect(G_OBJECT(item_config), "activate", G_CALLBACK(configure_main), &gx);

	item = gtk_menu_item_new_with_label("stocks list");
	gtk_widget_show(item);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(configure_main), &gx);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event", G_CALLBACK(show_popup), popup_menu);
}

GdkPixbuf *create_logo(const char *filename)
{
	GdkPixbuf *pb;
	GError *error = NULL;

	pb = gdk_pixbuf_new_from_file(filename, &error);
	if(!pb) {
		pr_err("create logo failed: %s\n", error->message);
		g_error_free(error);
	}
	return pb;
}

int main_ui(int argc, char *argv[], struct sm_xmlcfg *smxc)
{
	int i;
	int px, py;
	int width, height;
	struct sm_stock *stock;
	int interval;

	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "stkmon");
	gtk_window_set_default_size(GTK_WINDOW(win), 0, 0);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	gtk_window_get_position(GTK_WINDOW(win), &px, &py);
	gtk_window_move(GTK_WINDOW(win), px * 2, py);
	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);
	gtk_window_set_icon(GTK_WINDOW(win), create_logo("logo.png"));

	GtkWidget *ebox = gtk_event_box_new();
	GtkWidget *mbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(win), ebox);
	gtk_container_add(GTK_CONTAINER(ebox), mbox);

	GtkWidget *menubar = create_menubar(win);
	gtk_box_pack_start(GTK_BOX(mbox), menubar, FALSE, FALSE, 0);

	create_popupmenu(ebox);

	GtkWidget *infopanel = ui_monitor_create_info_panel(smxc);
	gtk_box_pack_start(GTK_BOX(mbox), infopanel, FALSE, FALSE, 0);

	interval = atoi(smxc->interval);
	if(interval <= 0)
		interval = 5000;
	g_timeout_add(interval, (GSourceFunc)hdlr_1s, (gpointer)smxc);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(win, "event", G_CALLBACK(main_event), NULL);

	gtk_widget_show_all(win);
	hdlr_1s((gpointer)smxc);

	gtk_window_set_opacity(GTK_WINDOW(win), WIN_OPACITY);
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	pr_info("%d, %d, %d, %d\n", px, py, width, height);
	gtk_window_move(GTK_WINDOW(win), px * 2 - width - 50, py * 2 - height - 80);

	gtk_main();

	return 0;
}

gboolean hdlr_1s(gpointer *p)
{
	char buffer[4096];
	char *bp;
	int i;
	struct sm_xmlcfg *smxc = (struct sm_xmlcfg *)p;
	struct sm_stock *stock;
	int rc;
	GdkColor color;
	static int count = 0;
	struct stk_stkdat *dat;

	rc = sinajs_pull_data(smxc->stock);
	if(rc < 0) {
		pr_err("sinajs_pull_data failed\n");
		return TRUE;
	}

	sinajs_apply_data(smxc);
	calc_realtime_info(smxc);
	calc_pr_info(smxc);

	for(stock = smxc->stock; stock; stock = stock->next) {
		if(!stock->pull_data)
			continue;

		dat = STK_GET_STKDAT(stock->pull_data);
		/* set price */
		sprintf(buffer, "%.2f", dat->price);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_price), buffer);

		/* set price raise */
		sprintf(buffer, "%.2f", stock->chg_rate * 100);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_raise), buffer);

		/* set min price relative rasie */
		float stop_profit = stock->cfg.stop_profit.f;
		float stop_loss = stock->cfg.stop_loss.f;
		float min_raise = stock->chg_rate_min;
		sprintf(buffer, "%.1f", min_raise * 100);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_trigger), buffer);

		if(min_raise >= stop_profit - 1)
			gdk_color_parse(COLOR_STOPP, &color);
		else if(min_raise <= stop_loss - 1)
			gdk_color_parse(COLOR_STOPL, &color);
		else
			gdk_color_parse(COLOR_EQ, &color);
		gtk_widget_modify_fg(stock->ui.label_trigger, GTK_STATE_NORMAL, &color);

		if(dat->price > dat->pre_close)
			gdk_color_parse(COLOR_RISE, &color);
		else if(dat->price < dat->pre_close)
			gdk_color_parse(COLOR_DROP, &color);
		else
			gdk_color_parse(COLOR_EQ, &color);
		gtk_widget_modify_fg(stock->ui.label_price, GTK_STATE_NORMAL, &color);
		gtk_widget_modify_fg(stock->ui.label_raise, GTK_STATE_NORMAL, &color);
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	struct sm_xmlcfg smxc;

	pr_pkg();

	pr_info("load configure.\n");
	load_xmlconfig(&smxc);

	pr_info("start main ui.\n");
	main_ui(argc, argv, &smxc);

	return 0;
}
