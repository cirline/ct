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
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 1);

	cairo_rectangle(cr, 20, 20, 100, 60);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_arc(cr, 200, 40, 20, 0, 2 * M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_arc(cr, 100, 200, 20, M_PI / 2, M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_translate(cr, 220, 180);
	cairo_scale(cr, 1, 0.7);
	cairo_arc(cr, 100, 200, 20, 0, 2 * M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_move_to(cr, 300, 300);
	cairo_line_to(cr, 400, 300);
	cairo_line_to(cr, 400, 400);
	cairo_close_path(cr);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_move_to(cr, 400, 200);
	cairo_line_to(cr, 400, 100);
	cairo_line_to(cr, 300, 100);
	cairo_curve_to(cr, 320, 110, 350, 120, 400, 200);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);

	cairo_pattern_t *pattern1 = cairo_pattern_create_for_surface(surface1);
	cairo_set_source(cr, pattern1);
	cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
	cairo_rectangle(cr, 500, 500, 100, 100);
	cairo_fill(cr);

	cairo_pattern_destroy(pattern1);

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
