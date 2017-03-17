#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
cairo_surface_t *g_sf;
gint gw, gh;
gboolean gtimer = TRUE;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(!gtimer)
		return FALSE;

	gtk_widget_queue_draw(widget);

	return TRUE;
}
#if 0
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(widget, cr);

	return FALSE;
}
#endif

static void setup(GtkWidget *win)
{
	GdkScreen *screen;
	GdkVisual *visual;

	gtk_widget_set_app_paintable(win, TRUE);
	gtk_window_set_type_hint(GTK_WINDOW(win), GDK_WINDOW_TYPE_HINT_DOCK);
//	gtk_window_set_keep_below(GTK_WINDOW(win), TRUE);

	screen = gdk_screen_get_default();
	visual = gdk_screen_get_rgba_visual(screen);

	if(visual != NULL && gdk_screen_is_composited(screen))
		gtk_widget_set_visual(win, visual);
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	g_image = cairo_image_surface_create_from_png("demo.png");

	gw = cairo_image_surface_get_width(g_image);
	gh = cairo_image_surface_get_height(g_image);

	g_sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, gw, gh);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	setup(win);

	GtkWidget *lbl = gtk_label_new("label message, aaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	gtk_container_add(GTK_CONTAINER(win), lbl);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(500, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

