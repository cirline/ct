
#include <cairo.h>
#include <gtk/gtk.h>

	static gboolean
on_expose_event (GtkWidget * widget,
		GdkEventExpose * event, gpointer data)
{
	cairo_t *cr;

	cr = gdk_cairo_create (GTK_WINDOW(widget));

	gint i;
	for (i = 1; i <= 10; i++) {
		cairo_set_source_rgba (cr, 0, 0, 1, i * 0.1);
		cairo_rectangle (cr, 50 * i, 20, 40, 40);
		cairo_fill (cr);
	}

	cairo_destroy (cr);

	return FALSE;
}

	int
main (int argc, char *argv[])
{
	GtkWidget *window;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	g_signal_connect (G_OBJECT (window), "expose-event",
			G_CALLBACK (on_expose_event), NULL);
	g_signal_connect (G_OBJECT (window), "destroy",
			G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_position (GTK_WINDOW (window),
			GTK_WIN_POS_CENTER);
	gtk_window_set_default_size (GTK_WINDOW (window), 590, 90);
	gtk_window_set_title (GTK_WINDOW (window), "transparency");

	gtk_widget_set_app_paintable (window, TRUE);
	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}

