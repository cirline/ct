#include <cairo.h>
#include <gtk/gtk.h>

struct {
	int count;
	double x[100], y[100];
} g;

static void do_drawing(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 10);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
	cairo_move_to(cr, 40, 30);
	cairo_line_to(cr, 200, 30);
	cairo_stroke(cr);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	cairo_move_to(cr, 40, 50);
	cairo_line_to(cr, 200, 50);
	cairo_stroke(cr);

	cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
	cairo_move_to(cr, 40, 70);
	cairo_line_to(cr, 200, 70);
	cairo_stroke(cr);

	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, 40, 30);
	cairo_line_to(cr, 40, 70);
	cairo_stroke(cr);

	cairo_move_to(cr, 200, 30);
	cairo_line_to(cr, 200, 70);
	cairo_stroke(cr);
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
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(win, "button-press-event", G_CALLBACK(clicked), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}
