#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

static void do_drawing(cairo_t *cr, gint x, gint w, gint h, cairo_operator_t op)
{
	cairo_t *cr1, *cr2;
	cairo_surface_t *sf1, *sf2;

	sf1 = cairo_surface_create_similar(cairo_get_target(cr), CAIRO_CONTENT_COLOR_ALPHA, w, h);
	sf2 = cairo_surface_create_similar(cairo_get_target(cr), CAIRO_CONTENT_COLOR_ALPHA, w, h);

	cr1 = cairo_create(sf1);
	cairo_set_source_rgb(cr1, 1, 0, 0);
	cairo_rectangle(cr1, x, 20, 50, 50);
	cairo_fill(cr1);

	cr2 = cairo_create(sf2);
	cairo_set_source_rgb(cr2, 0, 0, 1);
	cairo_rectangle(cr2, x + 10, 40, 50, 50);
	cairo_fill(cr2);

	// compositing
	cairo_set_operator(cr1, op);	// if not set this, cr2 only cover(default op?) cr1
	cairo_set_source_surface(cr1, sf2, 0, 0);	// sf2 add to cr1
	cairo_paint(cr1);

	// draw it
	cairo_set_source_surface(cr, sf1, 0, 0);
	cairo_paint(cr);

	cairo_surface_destroy(sf1);
	cairo_surface_destroy(sf2);

	cairo_destroy(cr1);
	cairo_destroy(cr2);
}

static gboolean timer_handler(GtkWidget *widget)
{
//	gtk_widget_queue_draw(widget);

	return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	cairo_operator_t op[] = {
		CAIRO_OPERATOR_DEST_OVER,
		CAIRO_OPERATOR_DEST_IN,
		CAIRO_OPERATOR_OUT,
		CAIRO_OPERATOR_ADD,
		CAIRO_OPERATOR_ATOP,
		CAIRO_OPERATOR_DEST_ATOP,
	};

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	gint i;
	gint x, y;

	for(x = y = 20, i = 0; i < 6; x += 80, i++) {
		do_drawing(cr, x, width, height, op[i]);
	}

	return FALSE;
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

	g_timeout_add(100, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

