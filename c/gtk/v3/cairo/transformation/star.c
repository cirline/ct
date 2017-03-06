#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;

int p[11][2] = {
	{0, 85},
	{75, 75},
	{100, 10},
	{125, 75},
	{150, 125},
	{160, 190},
	{100, 150},
	{40, 190},
	{50, 125},
	{0, 85},
};

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	static gdouble angle = 0;
	static gdouble scale = 1;
	static gdouble delta = 0.01;

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint w, h;
	gtk_window_get_size(GTK_WINDOW(win), &w, &h);

	cairo_set_line_width(cr, 0.5);
	cairo_translate(cr, w / 2, h / 2);
	cairo_set_source_rgb(cr, 0, 0, 1);
	cairo_rotate(cr, angle);

	cairo_scale(cr, scale, scale);

	gint i;

#if 0
	for(i = 0; i < 10; i++)
		cairo_line_to(cr, p[i][0], p[i][1]);

	cairo_close_path(cr);
#endif
	cairo_rectangle(cr, 0, 0, 80, 80);

	cairo_fill(cr);
	cairo_stroke(cr);

	if(scale < 0.01)
		delta = - delta;
	else if(scale > 0.99)
		delta = - delta;

	scale += delta;
	angle += 0.01;

}

static gboolean timer_handler(GtkWidget *widget)
{
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
	gint width = cairo_image_surface_get_width(g_image);
	gint height = cairo_image_surface_get_height(g_image);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(200, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

