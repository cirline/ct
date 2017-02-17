#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

double alpha = 1;
int size = 1;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 0.2);
	cairo_paint(cr);	// filled background

	cairo_move_to(cr, 0, 100);

	cairo_set_font_size(cr, size);
	cairo_set_source_rgb(cr, 1, 1, 1);

	cairo_text_path(cr, "ZetCode");		// get a path by text
	cairo_clip(cr);		// select the path (restrict by path)

	cairo_paint_with_alpha(cr, alpha);	// set the path content

	size += 2;
	alpha -= 0.01;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(widget, cr);

	return FALSE;
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(alpha < 0)
		return FALSE;

	gtk_widget_queue_draw(widget);

	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(14, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}
