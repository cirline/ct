#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
gint arr_index = 0;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	gint x = width / 2;
	gint y = height / 2;

	gint delta = arr_index & 1 ? 50 : -50;

	cairo_set_source_surface(cr, g_image, 1, 1);
	cairo_arc(cr, x + delta, y, 50, 0, 2 * M_PI);
	cairo_clip(cr);

	cairo_paint(cr);

}

static gboolean timer_handler(GtkWidget *widget)
{
	arr_index++;
	gtk_widget_queue_draw(widget);

	return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(widget, cr);

	return FALSE;
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	g_image = cairo_image_surface_create_from_png("aaa.png");
	gint width = cairo_image_surface_get_width(g_image);
	gint height = cairo_image_surface_get_height(g_image);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(1000, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

