#include <gtk/gtk.h>

#include "stkmon.h"

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

GtkWidget *ui_monitor_create_info_panel(struct sm_xmlcfg *smxc)
{
	struct sm_stock *stock;
	GtkWidget *vbox;

	vbox = gtk_vbox_new(FALSE, 1);

	for(stock = smxc->stock; stock; stock = stock->next) {
		GtkWidget *align;
		GtkWidget *label;

		if(!stock->visible)
			continue;

		GtkWidget *hbox = gtk_hbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

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
		gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 2, 2);
		label = gtk_label_new("0.00");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_trigger = label;
	}

	return vbox;
}
