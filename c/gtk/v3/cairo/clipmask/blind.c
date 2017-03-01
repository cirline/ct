#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
cairo_surface_t *g_sf;

gboolean timer;
gint g_width, g_height;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	static gint h = 0;

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	gint x = width / 2;
	gint y = height / 2;

	cairo_t *ic = cairo_create(g_sf);
	cairo_rectangle(ic, 0, 0, g_width, h);
	cairo_fill(ic);

	h++;
	if(h == g_height)
		timer = FALSE;

	cairo_set_source_surface(cr, g_image, 10, 10);
	cairo_mask_surface(cr, g_sf, 10, 10);
	cairo_destroy(ic);
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(!timer)
		return FALSE;

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

	g_image = cairo_image_surface_create_from_png("image.png");
	g_width = cairo_image_surface_get_width(g_image);
	g_height = cairo_image_surface_get_height(g_image);
	g_sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, g_width, g_height);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_default_size(GTK_WINDOW(win), 325, 250);

	g_timeout_add(200, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_sf);
	cairo_surface_destroy(g_image);

	return 0;
}

