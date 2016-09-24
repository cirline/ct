#include <gtk/gtk.h>

#include "stkmon.h"

int parse_xmlconfig(struct sm_desc *);
int save_xmlconfig(struct sm_xmlcfg *);

struct {
	GtkWidget *entry_delay_ms;
	GtkWidget *entry_code;
	GtkWidget *entry_stkex;
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

GtkWidget *create_stocks_list(struct sm_xmlcfg *smxc)
{
	GtkListStore *store;
	struct sm_stock *stock;

	GtkWidget *list = gtk_tree_view_new();
	gtk_widget_set_size_request(list, 120, 120);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

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
	add_list(list, "add ...");

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	return list;
}

void configure_main(GtkWidget *widget, gpointer p)
{
	GtkWidget *label;
	GtkWidget *table;
	GtkWidget *hbox;
	GtkWidget *align;
	struct sm_desc desc;
	char buffer[1024];
	struct sm_xmlcfg smxc;
	int curcol;

	g_printf("%s\n", __func__);
	parse_xmlconfig(&desc);
	load_xmlconfig(&smxc);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "configure");
	gtk_window_set_default_size(GTK_WINDOW(win), 0, 0);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	GtkWidget *mbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(win), mbox);

	table = gtk_table_new(1, 2, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);
	gtk_box_pack_start(GTK_BOX(mbox), table, FALSE, FALSE, 1);

	curcol = 0;
	label = gtk_label_new("interval(ms): ");
	ui.entry_delay_ms = gtk_entry_new();
	sprintf(buffer, "%d", desc.cfg.delay_ms);
	gtk_entry_set_text(GTK_ENTRY(ui.entry_delay_ms), buffer);

	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, curcol, curcol + 1);
	gtk_table_attach_defaults(GTK_TABLE(table), ui.entry_delay_ms, 1, 2, curcol, curcol + 1);

	table = gtk_table_new(4, 4, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);
	gtk_box_pack_start(GTK_BOX(mbox), table, FALSE, FALSE, 1);

	curcol = 0;
	GtkWidget *stocks_list = create_stocks_list(&smxc);
	gtk_table_attach_defaults(GTK_TABLE(table), stocks_list, 0, 1, curcol, curcol + 2);
	label = gtk_label_new("code");
	ui.entry_code = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, curcol, curcol + 1);
	gtk_table_attach_defaults(GTK_TABLE(table), ui.entry_code, 2, 3, curcol, curcol + 1);
	label = gtk_label_new("stkex");
	ui.entry_stkex = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 2, curcol + 1, curcol + 2);
	gtk_table_attach_defaults(GTK_TABLE(table), ui.entry_stkex, 2, 3, curcol + 1, curcol + 2);

	align = gtk_alignment_new(1, 0, 0, 0);
	hbox = gtk_hbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(align), hbox);
	gtk_box_pack_start(GTK_BOX(mbox), align, FALSE, FALSE, 1);

	GtkWidget *btn_add = gtk_button_new_with_mnemonic("_Add");
	gtk_container_add(GTK_CONTAINER(hbox), btn_add);

	GtkWidget *btn_update = gtk_button_new_with_mnemonic("_Update");
	gtk_container_add(GTK_CONTAINER(hbox), btn_update);

	GtkWidget *btn_delete = gtk_button_new_with_mnemonic("_Delete");
	gtk_container_add(GTK_CONTAINER(hbox), btn_delete);

	GtkWidget *btn_ok = gtk_button_new_with_mnemonic("_OK");
	g_signal_connect(G_OBJECT(btn_ok), "clicked", G_CALLBACK(configure_save), NULL);
	gtk_container_add(GTK_CONTAINER(hbox), btn_ok);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(win);
	gtk_main();
}

