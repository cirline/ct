#include <unistd.h>
#include <gtk/gtk.h>

gboolean timer_handler(GtkWidget *widget)
{
	g_printf("%s -- pid = %d\n", __func__, getpid());

	GDateTime *now = g_date_time_new_now_local();
	gchar *my_time = g_date_time_format(now, "%H:%M:%S");

	gtk_label_set_text(GTK_LABEL(widget), my_time);

	g_free(my_time);
	g_date_time_unref(now);

	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	g_printf("gtk event!, pid = %d\n", getpid());
	g_printf("%s -- pid = %d\n", __func__, getpid());

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 300, 200);

	GtkWidget *label = gtk_label_new("hello label");
	gtk_container_add(GTK_CONTAINER(win), label);

	g_timeout_add(1000, (GSourceFunc)timer_handler, (gpointer)label);

	g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(win));

	gtk_widget_show_all(win);
	timer_handler(win);

	gtk_main();
	return 0;
}
