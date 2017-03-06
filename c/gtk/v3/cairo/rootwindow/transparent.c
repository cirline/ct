#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
cairo_surface_t *g_sf;
gint gw, gh;
gboolean gtimer = TRUE;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	//cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 0.1);
	cairo_set_source_surface(cr, g_image, 0, 0);
	cairo_paint(cr);
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(!gtimer)
		return FALSE;

	gtk_widget_queue_draw(widget);

	return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(widget, cr);

	return FALSE;
}

static void tran_setup(GtkWidget *win)
{
	GdkScreen *screen;
	GdkVisual *visual;

	gtk_widget_set_app_paintable(win, TRUE);
	screen = gdk_screen_get_default();

	// visual contains the low level display information
	visual = gdk_screen_get_rgba_visual(screen);

	// not all displays support this operation, so check it
	if(visual != NULL && gdk_screen_is_composited(screen))
		gtk_widget_set_visual(win, visual);
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	g_image = cairo_image_surface_create_from_png("image.png");

	gw = cairo_image_surface_get_width(g_image);
	gh = cairo_image_surface_get_height(g_image);

	g_sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, gw, gh);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	tran_setup(win);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(500, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

