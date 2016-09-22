#include <gtk/gtk.h>

#include "stkmon.h"

int parse_xmlconfig(struct sm_desc *);
int save_xmlconfig(struct sm_xmlcfg *);

struct {
	GtkWidget *entry_delay_ms;
} ui;

struct sm_xmlcfg smxc;

void configure_save(GtkWidget *widget, gpointer p)
{
	g_printf("%s\n", __func__);

	smxc.delay_ms = (char *)gtk_entry_get_text(GTK_ENTRY(ui.entry_delay_ms));

	save_xmlconfig(&smxc);
}

void add_list(GtkWidget *list, unsigned char *str)
{
	GtkListStore *store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, str, -1);
}

void load_stocks(GtkWidget *box, struct sm_xmlcfg *smxc)
{
	GtkListStore *store;
	struct sm_stock *stock;

	GtkWidget *list = gtk_tree_view_new();
	gtk_box_pack_start(GTK_BOX(box), list, FALSE, FALSE, 0);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
			"stocks list", renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
	g_object_unref(store);


	for(stock = smxc->stock; stock; stock = stock->next) {
		add_list(list, stock->code);
	}

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
}

void configure_main(GtkWidget *widget, gpointer p)
{
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *align;
	struct sm_desc desc;
	char buffer[1024];
	struct sm_xmlcfg smxc;

	g_printf("%s\n", __func__);
	parse_xmlconfig(&desc);
	load_xmlconfig(&smxc);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "configure");
	gtk_window_set_default_size(GTK_WINDOW(win), 0, 0);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	GtkWidget *mbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(win), mbox);

	hbox = gtk_hbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(mbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new("interval(ms): ");
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	ui.entry_delay_ms = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox), ui.entry_delay_ms, FALSE, FALSE, 0);
	sprintf(buffer, "%d", desc.cfg.delay_ms);
	gtk_entry_set_text(GTK_ENTRY(ui.entry_delay_ms), buffer);

	hbox = gtk_hbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(mbox), hbox, FALSE, FALSE, 0);
	load_stocks(hbox, &smxc);

	align = gtk_alignment_new(1, 0, 0, 0);
	hbox = gtk_hbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(align), hbox);
	gtk_box_pack_start(GTK_BOX(mbox), align, FALSE, FALSE, 0);

	GtkWidget *btn_ok = gtk_button_new_with_mnemonic("_OK");
	GtkWidget *btn_apply = gtk_button_new_with_mnemonic("_Apply");
	g_signal_connect(G_OBJECT(btn_ok), "clicked", G_CALLBACK(configure_save), NULL);
	gtk_container_add(GTK_CONTAINER(hbox), btn_ok);
	gtk_container_add(GTK_CONTAINER(hbox), btn_apply);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(win);
	gtk_main();
}
