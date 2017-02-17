#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

struct {
	int count;
	double x[100], y[100];
} g;

cairo_surface_t *surface1;

static void do_drawing(cairo_t *cr)
{
	cairo_pattern_t *pat;

	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_set_line_width(cr, 12);
	cairo_translate(cr, 60, 60);

	pat = cairo_pattern_create_radial(30, 30, 10, 30, 30, 80);
	cairo_pattern_add_color_stop_rgba(pat, 0, 1, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba(pat, 100, 0, 0, 1, 1);
	cairo_set_source(cr, pat);

	cairo_arc(cr, 0, 0, 60, 0, M_PI * 2);
	cairo_fill(cr);

	cairo_translate(cr, 0, 120);
	pat = cairo_pattern_create_radial(0, 0, 10, 0, 0, 80);
	cairo_pattern_add_color_stop_rgba(pat, 0, 1, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba(pat, 0.3, 0, 1, 0, 1);
	cairo_pattern_add_color_stop_rgba(pat, 0.7, 0, 0, 1, 1);
	cairo_set_source(cr, pat);

	cairo_arc(cr, 0, 0, 60, 0, M_PI * 2);
	cairo_fill(cr);

	cairo_pattern_destroy(pat);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(cr);

	return FALSE;
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer p)
{
	printf("event button = %d\n", event->button);
	if(event->button == 1) {
		if(g.count > 99)
			g.count = 0;
		g.x[g.count] = event->x;
		g.y[g.count] = event->y;
		g.count++;
		gtk_widget_queue_draw(widget);
	} else if(event->button == 3) {
		g.count = 0;
		gtk_widget_queue_draw(widget);
	}


	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	g.count = 0;

	gtk_init(&argc, &argv);

	surface1 = cairo_image_surface_create_from_png("image.png");

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(win, "button-press-event", G_CALLBACK(clicked), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(surface1);

	return 0;
}
