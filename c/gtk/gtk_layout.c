#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 300, 200);
#if 0
	GtkWidget *fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(win), fixed);

	GtkWidget *btn1 = gtk_button_new_with_label("B1");
	gtk_fixed_put(GTK_FIXED(fixed), btn1, 150, 50);
	gtk_widget_set_size_request(btn1, 80, 30);

	GtkWidget *btn2 = gtk_button_new_with_label("B2");
	gtk_fixed_put(GTK_FIXED(fixed), btn2, 30, 30);
	gtk_widget_set_size_request(btn2, 80, 30);
#elif 0
	GtkWidget *align = gtk_alignment_new(0, 1, 0, 0);
	GtkWidget *lbl = gtk_label_new("bottom-left");

	gtk_container_add(GTK_CONTAINER(align), lbl);
	gtk_container_add(GTK_CONTAINER(win), align);
#elif 1
	GtkWidget *vbox = gtk_vbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	GtkWidget *settings = gtk_button_new_with_label("Settings");
	GtkWidget *accounts = gtk_button_new_with_label("Accounts");

	gtk_box_pack_start(GTK_BOX(vbox), settings, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), accounts, TRUE, TRUE, 0);
#endif

	g_signal_connect(GTK_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

