#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <ccutils/net.h>
#include <ccutils/log.h>

#include "sinajs.h"
#include "stkmon.h"
#include "stkxml.h"

static int gx = 99;

gboolean hdlr_1s(gpointer *);
int parse_xmlconfig(struct sm_desc *);
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

int main_ui(int argc, char *argv[], struct sm_desc *desc, struct sm_xmlcfg *smxc)
{
	int i;
	int px, py;
	struct sm_stock *stock;

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

	for(stock = smxc->stock; stock; stock = stock->next) {
		GtkWidget *align;
		GtkWidget *label;

		GtkWidget *hbox = gtk_hbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(mbox), hbox, FALSE, FALSE, 0);

		align = gtk_alignment_new(1, 0, 0.6, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 2, 2);
		label = gtk_label_new("0");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_price = label;

		align = gtk_alignment_new(1, 0, 0.4, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 2, 2);
		label = gtk_label_new("0.00");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_raise = label;

		hbox = gtk_hbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(mbox), hbox, FALSE, FALSE, 0);
		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 2, 2);
		label = gtk_label_new("0.00");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_trigger = label;
	}

	g_timeout_add(desc->cfg.delay_ms, (GSourceFunc)hdlr_1s, (gpointer)smxc);
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

		pr_info("stock = %s\n", sdp->code);

		sprintf(buffer, "%.2f", sdp->price);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_price), buffer);
		float raise = (sdp->price - sdp->pre_close) / sdp->pre_close * 100;
		sprintf(buffer, "%.2f", raise);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_raise), buffer);

		float last_minprice = atof(stock->last_minprice);
		float trigger = atof(stock->trigger);
		float trigger_percent = (sdp->price - last_minprice * trigger) / sdp->price;
		sprintf(buffer, "%.2f", trigger_percent);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_trigger), buffer);
		if(trigger_percent > -0.01)
			gdk_color_parse(COLOR_RISE, &color);
		else
			gdk_color_parse(COLOR_DROP, &color);
		gtk_widget_modify_fg(stock->ui.label_trigger, GTK_STATE_NORMAL, &color);

		if(sdp->price > sdp->pre_close)
			gdk_color_parse(COLOR_RISE, &color);
		else if(sdp->price < sdp->pre_close)
			gdk_color_parse(COLOR_DROP, &color);
		else
			gdk_color_parse(COLOR_EQ, &color);
		gtk_widget_modify_fg(stock->ui.label_price, GTK_STATE_NORMAL, &color);
		gtk_widget_modify_fg(stock->ui.label_raise, GTK_STATE_NORMAL, &color);
		/*
		pr_info("code = %s, price = %.2f, pre_close = %.2f, trigger = %.2f\n",
				data.code, data.price, data.pre_close, trigger);
				*/
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	struct sm_desc desc;	/* deprecated */
	struct sm_xmlcfg smxc;

	parse_xmlconfig(&desc);	/* deprecated */
	load_xmlconfig(&smxc);
	main_ui(argc, argv, &desc, &smxc);

	return 0;
}
