#include <stdio.h>
#include <gtk/gtk.h>

#include "stkmon/stkmon.h"

int ui_show_dialog_info(GtkWidget *win, char *msg)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(win),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			msg);
	gtk_window_set_title(GTK_WINDOW(dialog), "info");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	return 0;
}

GtkWidget *ui_monitor_create_dynamic_table(struct stk_xmlcfg *sxc)
{
	struct stk_stock *stock;
	int tbl_cur_line = 0;
	struct ge_index *idx;

	GtkWidget *table = gtk_table_new(0, 0, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);

	for(stock = sxc->stock_list.cqh_first; stock != (void *)&sxc->stock_list;
			stock = stock->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!stock->visible)
			continue;

		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 2, 0, 2, 2);
		label = gtk_label_new("0");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_table_attach_defaults(GTK_TABLE(table), align, 0, 1, tbl_cur_line, tbl_cur_line + 1);
		stock->ui.label_name = label;

		tbl_cur_line++;
	}

	for(idx = sxc->index_list.cqh_first; idx != (void *)&sxc->index_list;
			idx = idx->list.cqe_next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!idx->visible)
			continue;

		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 2, 0, 2, 2);
		label = gtk_label_new("0");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_table_attach_defaults(GTK_TABLE(table), align, 0, 1, tbl_cur_line, tbl_cur_line + 1);
		idx->ui.label_name = label;

		tbl_cur_line++;
	}

	return table;
}

