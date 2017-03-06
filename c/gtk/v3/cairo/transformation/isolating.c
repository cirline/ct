#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
gint arr_index = 0;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 1);
	cairo_rectangle(cr, 20, 30, 80, 50);
	cairo_stroke(cr);

	cairo_rectangle(cr, 120, 30, 80, 50);
	cairo_stroke(cr);

	cairo_rectangle(cr, 120, 130, 80, 50);
	cairo_stroke(cr);

	cairo_save(cr);	// save the cr properity

	cairo_scale(cr, 0.8, 1);
	cairo_set_source_rgb(cr, 0, 1, 0);
	cairo_rectangle(cr, 120, 30, 80, 50);
	cairo_fill(cr);

	cairo_restore(cr);	// restore the cr porperity

	cairo_save(cr);

	cairo_scale(cr, 1.2, 1);
	cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_rectangle(cr, 120, 130, 80, 50);
	cairo_fill(cr);

	cairo_restore(cr);

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

	g_timeout_add(1000, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

