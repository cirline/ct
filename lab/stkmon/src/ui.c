#include <gtk/gtk.h>

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
