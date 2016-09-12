#include <gtk/gtk.h>

gchar buf[256];

gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr;

	cr = gdk_cairo_create(widget->window);
	cairo_move_to(cr, 30, 30);
	cairo_set_font_size(cr, 15);
	cairo_show_text(cr, buf);

	cairo_destroy(cr);

	return FALSE;
}

gboolean timer_handler(GtkWidget *widget)
{
	if(widget->window == NULL)
		return FALSE;

	GDateTime *now = g_date_time_new_now_local();
	gchar *my_time = g_date_time_format(now, "%H:%M:%S");

	g_sprintf(buf, "%s", my_time);

	g_free(my_time);
	g_date_time_unref(now);
	gtk_widget_queue_draw(widget);

	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	g_printf("gtk event!\n");

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 300, 200);

	GtkWidget *darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);
	g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_timeout_add(1000, (GSourceFunc)timer_handler, (gpointer)win);

	g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(win));

	gtk_widget_show_all(win);
	timer_handler(win);

	gtk_main();
	return 0;
}
