//#define DEBUG
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
#include "stkmon/stock_list.h"
#include "geye/monitor.h"
#include "geye/ge.h"
#include "geye/ui.h"
#include "config.h"

static int gx = 99;

//void configure_main(GtkWidget *widget, gpointer p);

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

	//v2 GtkWidget *quit_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_grp);
	GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
	//v2 gtk_widget_add_accelerator(quit_item, "activate", accel_grp, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(quit_item), "activate", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *item_config = gtk_menu_item_new_with_label("configure");
	//g_signal_connect(G_OBJECT(item_config), "activate", G_CALLBACK(configure_main), &gx);

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

void create_popupmenu(GtkWidget *ebox, struct golden_eye *ge)
{
	GtkWidget *item;

	GtkWidget *popup_menu = gtk_menu_new();

	GtkWidget *item_config = gtk_menu_item_new_with_label("configure");
	gtk_widget_show(item_config);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item_config);
	//g_signal_connect(G_OBJECT(item_config), "activate", G_CALLBACK(configure_main), &gx);

	item = gtk_menu_item_new_with_label("stocks list");
	gtk_widget_show(item);
	gtk_menu_shell_append(GTK_MENU_SHELL(popup_menu), item);
	//g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(configure_main), &gx);

	g_signal_connect_swapped(G_OBJECT(ebox), "button-press-event", G_CALLBACK(show_popup), popup_menu);
}

/**
 *  _____ win __________________________
 * / ____ ebox _________________________
 * |/ ___ mbox ________________________
 * ||/  _ menu ________
 * ||| /               \
 * ||| \_______________/
 * |||  __ monitor ____________________
 * ||| / _ fixed ___    _ dynamic ___  \
 * ||| |/           \  /             \ |
 * ||| ||           |  |             | |
 * ||| |\___________/  \_____________/ |
 * ||| \_______________________________/
 * |||
 * ||`---------------------------------
 * |`----------------------------------
 * `-----------------------------------
 */
int main_ui(int argc, char *argv[], struct sm_xmlcfg *smxc)
{
	int i;
	int px, py;
	int width, height;
	struct sm_stock *stock;
	int interval;

	/* win */
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	/* ebox and mbox */
	GtkWidget *ebox = gtk_event_box_new();
	GtkWidget *mbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(win), ebox);
	gtk_container_add(GTK_CONTAINER(ebox), mbox);

	/* menu */
	GtkWidget *menubar = create_menubar(win);
	gtk_box_pack_start(GTK_BOX(mbox), menubar, FALSE, FALSE, 0);

	create_popupmenu(ebox, smxc);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

void on_window1_destroy(GtkWidget *widget, gpointer data)
{
	pr_info("%s\n", __func__);
	gtk_main_quit();
}

int old_main(GtkApplication *app, struct golden_eye_2 *ge)
{
	struct golden_eye *ss = &ge->old;

	pr_pkg();
	pr_info("GTK v%d.%d.%d\n", gtk_major_version, gtk_minor_version, gtk_micro_version);
	pr_info("Glib v%d.%d.%d\n", glib_major_version, glib_minor_version, glib_micro_version);

	strcpy(ge->version, PACKAGE_STRING);

	pr_info("load configure.\n");
	load_xmlconfig(ss);

	ss->pull_index_data = sinajs_pull_index_data;

	pr_info("start main ui.\n");
	ui_window_start(app, ge);

	return 0;
}

