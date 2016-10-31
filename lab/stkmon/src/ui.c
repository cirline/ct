#include <gtk/gtk.h>

#include "stkmon/stkmon.h"

int ui_show_dialog_info(GtkWidget *widget, GtkWidget *win, char *msg)
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

/*
 * monitor info panel
 * ---------------------
 *  cur_price     avg_price
 *  day_raise     last_action_raise
 * ---------------------
 */
GtkWidget *ui_monitor_create_info_panel(struct sm_xmlcfg *smxc)
{
	struct sm_stock *stock;
	GtkWidget *vbox;
	int tbl_cur_line = 0;

	GtkWidget *table = gtk_table_new(0, 0, FALSE);
	//gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	//gtk_table_set_col_spacings(GTK_TABLE(table), 5);

	for(stock = smxc->stock; stock; stock = stock->next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!stock->visible)
			continue;

		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 2, 0, 2, 2);
		label = gtk_label_new("0");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_table_attach_defaults(GTK_TABLE(table), align, 0, 1, tbl_cur_line, tbl_cur_line + 1);
		stock->ui.label_price = label;

		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 2, 0, 2, 2);
		label = gtk_label_new("0.00");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_table_attach_defaults(GTK_TABLE(table), align, 1, 2, tbl_cur_line, tbl_cur_line + 1);
		stock->ui.label_raise = label;

		tbl_cur_line++;

		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 2, 2, 2);
		label = gtk_label_new("0.00");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_table_attach_defaults(GTK_TABLE(table), align, 1, 2, tbl_cur_line, tbl_cur_line + 1);
		stock->ui.label_trigger = label;

		tbl_cur_line++;
	}

	return table;
}
