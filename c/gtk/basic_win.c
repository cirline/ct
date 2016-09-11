#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

struct base_gui {
	struct {
		GtkWidget *this;
		GtkToolItem *undo;
		GtkToolItem *redo;
	} toolbar;
	GtkWidget statusbar;
} bgui;
static GtkWidget *statusbar;

void toggle_statusbar(GtkWidget *widget, gpointer win)
{
	g_printf("%s\n", __func__);

	if(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)))
		gtk_widget_show(statusbar);
	else
		gtk_widget_hide(statusbar);
}

void print_msg(GtkWidget *widget, gpointer win)
{
	g_printf("button clicked\n");
}

void cb_toolbar(GtkWidget *widget, gpointer win)
{
	static int count = 2;
	const char *name = gtk_widget_get_name(widget);

	if(strcmp(name, "undo") == 0)
		count++;
	else
		count--;

	g_printf("name = %s, count = %d\n", name, count);

	if(count < 0) {
		gtk_widget_set_sensitive(widget, FALSE);
		gtk_widget_set_sensitive((gpointer)bgui.toolbar.undo, TRUE);
	}
	if(count > 5) {
		gtk_widget_set_sensitive(widget, FALSE);
		gtk_widget_set_sensitive((gpointer)bgui.toolbar.redo, TRUE);
	}
}

/**
 * menubar	-> item_file	-> filemenu	-> item_impr	-> impr_menu	-> item_impr_feed
 *										-> item_impr_book
 *						-> item_quit
 *
 */
GtkWidget *create_menubar(GtkWidget *win)
{
	GtkWidget *sep = gtk_separator_menu_item_new();

	GtkAccelGroup *accel_grp = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(win), accel_grp);

	// menu bar
	GtkWidget *menubar = gtk_menu_bar_new();
	GtkWidget *item_file = gtk_menu_item_new_with_label("File");
	/**
	 * mnemonic menu
	 */
	GtkWidget *item_view = gtk_menu_item_new_with_mnemonic("_View");
	GtkWidget *item_help = gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item_file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item_view);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item_help);

	// file menu
	GtkWidget *filemenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_file), filemenu);
	GtkWidget *item_impr = gtk_menu_item_new_with_label("Import");
	/**
	 * image & accelerator menu
	 */
	GtkWidget *item_quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_grp);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), item_impr);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), item_quit);

	gtk_widget_add_accelerator(item_quit, "activate", accel_grp, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(G_OBJECT(item_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);

	// file -> import menu
	GtkWidget *impr_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_impr), impr_menu);
	GtkWidget *item_impr_feed = gtk_menu_item_new_with_label("Import new feed...");
	GtkWidget *item_impr_book = gtk_menu_item_new_with_label("Import bookmarks...");
	gtk_menu_shell_append(GTK_MENU_SHELL(impr_menu), item_impr_feed);
	gtk_menu_shell_append(GTK_MENU_SHELL(impr_menu), item_impr_book);

	// view menu
	GtkWidget *view_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item_view), view_menu);
	/**
	 * check menu item
	 */
	GtkWidget *item_statusbar = gtk_check_menu_item_new_with_label("Statusbar");
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), item_statusbar);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item_statusbar), TRUE);

	g_signal_connect(G_OBJECT(item_statusbar), "activate", G_CALLBACK(toggle_statusbar), NULL);

	return menubar;
}

static GtkWidget * create_toolbar(GtkWidget *win)
{
	GtkToolItem *item;
	GtkToolItem *sep = gtk_separator_tool_item_new();

	GtkWidget *toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	item = gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
	gtk_widget_set_name(GTK_WIDGET(item), "undo");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	g_signal_connect(G_OBJECT(item), "clicked", G_CALLBACK(cb_toolbar), item);
	bgui.toolbar.undo = item;

	item = gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
	gtk_widget_set_name(GTK_WIDGET(item), "redo");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	g_signal_connect(G_OBJECT(item), "clicked", G_CALLBACK(cb_toolbar), item);
	bgui.toolbar.redo = item;

	GtkToolItem *save_tb = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save_tb, -1);

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);

	GtkToolItem *quit_tb = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), quit_tb, -1);

	g_signal_connect(G_OBJECT(quit_tb), "clicked", G_CALLBACK(gtk_main_quit), NULL);

	return toolbar;
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *btn, *btn2;
	GtkWidget *halign, *halign2;

	gtk_init(&argc, &argv);
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "center");
	gtk_window_set_default_size(GTK_WINDOW(win), 250, 100);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	gtk_container_set_border_width(GTK_CONTAINER(win), 15);

	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	// menubar
	GtkWidget *menubar = create_menubar(win);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

	// toolbar
	GtkWidget *toolbar = create_toolbar(win);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

	// statusbar
	statusbar = gtk_statusbar_new();
	gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);
#if 1
	btn = gtk_button_new_with_label("Button");
	gtk_widget_set_tooltip_text(btn, "Button tooltip");
	halign = gtk_alignment_new(0, 0, 0, 0);

	gtk_container_add(GTK_CONTAINER(halign), btn);

	btn2 = gtk_button_new_with_mnemonic("_Button2");
	g_signal_connect(btn2, "clicked", G_CALLBACK(print_msg), NULL);

	halign = gtk_alignment_new(0, 50, 0, 0);
	gtk_container_add(GTK_CONTAINER(halign), btn2);
	gtk_container_add(GTK_CONTAINER(win), halign);
#endif
	gtk_widget_show_all(win);

	gtk_widget_show(win);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	return 0;
}

