#define pr_fmt(fmt)	"slist   ] " fmt

#include <gtk/gtk.h>

#include <ccutils/log.h>

#include "stkmon/stkmon.h"

int save_xmlconfig(struct sm_xmlcfg *);

struct {
	GtkWidget *entry_delay_ms;
	GtkWidget *entry_code;
	GtkWidget *entry_stkex;
} ui;

struct sm_xmlcfg smxc;

void slist_main_window(GtkWidget *widget, gpointer p)
{
	GtkBuilder *builder;
	GtkWidget *win;

	pr_info("%s\n", __func__);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "layout/stock_list.glade", NULL);
	win = GTK_WIDGET(gtk_builder_get_object(builder, "slist_main"));
	gtk_builder_connect_signals(builder, NULL);

	GtkWidget *label = gtk_label_new("test label");
	GtkWidget *mbox = GTK_WIDGET(gtk_builder_get_object(builder, "slist_mbox"));
	gtk_box_pack_start(GTK_BOX(mbox), label, FALSE, FALSE, 0);

	g_object_unref(G_OBJECT(builder));
	gtk_widget_show_all(win);
	gtk_main();

}

void configure_save(GtkWidget *widget, gpointer p)
{
	g_printf("%s\n", __func__);

	//smxc.delay_ms = (char *)gtk_entry_get_text(GTK_ENTRY(ui.entry_delay_ms));

	save_xmlconfig(&smxc);
}

GtkWidget *creat_stocks_list(struct sm_xmlcfg *smxc)
{
	int count;
	int i;
	int tbl_cur_line;
	char *tbl_title[] = {"visible", "stkex", "code", "avg_p"};
	struct sm_stock *stock;

	count = smxc->stocks_count;
	pr_info("stocks count = %d\n", count);

	GtkWidget *table = gtk_table_new(ARRAY_SIZE(tbl_title), count, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);

	/* set stocks table title */
	tbl_cur_line = 0;
	for(i = 0; i < ARRAY_SIZE(tbl_title); i++) {
		GtkWidget *label = gtk_label_new(tbl_title[i]);
		gtk_table_attach_defaults(GTK_TABLE(table), label, i, i + 1, tbl_cur_line, tbl_cur_line + 1);
	}

	for(stock = smxc->stock_list.cqh_first; stock != (void *)&smxc->stock_list;
			stock = stock->list.cqe_next) {
		int col;
		GtkWidget *widget;

		tbl_cur_line++;
		col = 0;
		widget = gtk_label_new(stock->visible ? "true" : "-");
		gtk_table_attach_defaults(GTK_TABLE(table), widget, col, col + 1, tbl_cur_line, tbl_cur_line + 1);

		col++;
		widget = gtk_label_new(stock->exchange);
		gtk_table_attach_defaults(GTK_TABLE(table), widget, col, col + 1, tbl_cur_line, tbl_cur_line + 1);

		col++;
		widget = gtk_label_new(stock->code);
		gtk_table_attach_defaults(GTK_TABLE(table), widget, col, col + 1, tbl_cur_line, tbl_cur_line + 1);

		col++;
		widget = gtk_label_new(stock->cfg.avg_price.c);
		gtk_table_attach_defaults(GTK_TABLE(table), widget, col, col + 1, tbl_cur_line, tbl_cur_line + 1);
	}

	return table;
}

GtkWidget *creat_action_bar(void)
{
	GtkWidget *align;

	align = gtk_alignment_new(1, 0, 0, 0);

	GtkWidget *hbox = gtk_hbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(align), hbox);

	GtkWidget *btn_add = gtk_button_new_with_mnemonic("_Add");
	gtk_container_add(GTK_CONTAINER(hbox), btn_add);

	GtkWidget *btn_update = gtk_button_new_with_mnemonic("_Update");
	gtk_container_add(GTK_CONTAINER(hbox), btn_update);

	GtkWidget *btn_delete = gtk_button_new_with_mnemonic("_Delete");
	gtk_container_add(GTK_CONTAINER(hbox), btn_delete);

	GtkWidget *btn_ok = gtk_button_new_with_mnemonic("_OK");
	gtk_container_add(GTK_CONTAINER(hbox), btn_ok);
	g_signal_connect(G_OBJECT(btn_ok), "clicked", G_CALLBACK(configure_save), NULL);

	return align;
}

void configure_main(GtkWidget *widget, gpointer p)
{
	GtkWidget *label;
	GtkWidget *table;
	GtkWidget *hbox;
	GtkWidget *align;
	char buffer[1024];
	struct sm_xmlcfg smxc;
	int curcol;

	g_printf("%s\n", __func__);
	load_xmlconfig(&smxc);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "configure");
	gtk_window_set_default_size(GTK_WINDOW(win), 0, 0);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	GtkWidget *mbox = gtk_vbox_new(FALSE, 1);
	gtk_container_add(GTK_CONTAINER(win), mbox);

#if 0
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
#endif
	/*
	 * stocks list table
	 */
	GtkWidget *stocks_list = creat_stocks_list(&smxc);
	gtk_box_pack_start(GTK_BOX(mbox), stocks_list, FALSE, FALSE, 1);

	/*
	 * action bar
	 */
	GtkWidget *action_bar = creat_action_bar();
	gtk_box_pack_start(GTK_BOX(mbox), action_bar, FALSE, FALSE, 1);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(win);
	gtk_main();
}

