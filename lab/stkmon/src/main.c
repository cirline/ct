#define pr_fmt(fmt)	"stkmon  ] " fmt

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <ccutils/net.h>
#include <ccutils/log.h>

#include "stkmon/ui.h"
#include "sinajs.h"
#include "stkmon/stkmon.h"
#include "stkxml.h"
#include "config.h"

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

void create_popupmenu(GtkWidget *ebox)
{
	GtkWidget *item;

	GtkWidget *popup_menu = gtk_menu_new();

	GtkWidget *item_config = gtk_menu_item_new_with_label("configure");
	gtk_widget_show(item_config);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item_config);
	g_signal_connect(G_OBJECT(item_config), "activate", G_CALLBACK(configure_main), &gx);

	item = gtk_menu_item_new_with_label("list detail");
	gtk_widget_show(item);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item);
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(configure_main), &gx);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event", G_CALLBACK(show_popup), popup_menu);
}

int main_ui(int argc, char *argv[], struct sm_xmlcfg *smxc)
{
	int i;
	int px, py;
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

	gtk_widget_show_all(win);
	hdlr_1s((gpointer)smxc);

	gtk_main();

	return 0;
}

gboolean hdlr_1s(gpointer *p)
{
	char buffer[4096];
	char *bp;
	struct sinajs *sdp;
	int i;
	struct sm_xmlcfg *smxc = (struct sm_xmlcfg *)p;
	struct sm_stock *stock;
	int rc;
	GdkColor color;

	rc = sinajs_pull_data(smxc->stock);
	if(rc < 0) {
		pr_err("sinajs_pull_data failed\n");
		return TRUE;
	}

	for(stock = smxc->stock; stock; stock = stock->next) {

		sdp = stock->pull_data;
		if(!sdp)
			continue;

		/* set price */
		sprintf(buffer, "%.2f", sdp->price);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_price), buffer);

		/* set price raise */
		float raise = (sdp->price - sdp->pre_close) / sdp->pre_close * 100;
		sprintf(buffer, "%.2f", raise);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_raise), buffer);

		/* set min price relative rasie */
		float last_minprice = atof(stock->last_minprice);
		float stop_profit = atof(stock->stop_profit);
		float stop_loss = atof(stock->stop_loss);
		float min_raise = (sdp->price - last_minprice) / last_minprice;
		sprintf(buffer, "%.1f", min_raise * 100);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_trigger), buffer);

		pr_info("mr = %f, sp = %f, sl = %f\n", min_raise, stop_profit, stop_loss);
		if(min_raise >= stop_profit - 1)
			gdk_color_parse(COLOR_STOPP, &color);
		else if(min_raise <= stop_loss - 1)
			gdk_color_parse(COLOR_STOPL, &color);
		else
			gdk_color_parse(COLOR_EQ, &color);
		gtk_widget_modify_fg(stock->ui.label_trigger, GTK_STATE_NORMAL, &color);

		if(sdp->price > sdp->pre_close)
			gdk_color_parse(COLOR_RISE, &color);
		else if(sdp->price < sdp->pre_close)
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
