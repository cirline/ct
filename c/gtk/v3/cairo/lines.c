#include <cairo.h>
#include <gtk/gtk.h>

struct {
	int count;
	double x[100], y[100];
} g;

static void do_drawing(cairo_t *cr)
{
	int i, j;

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	for(i = 0; i < g.count; i++)
		for(j = 0; j < g.count; j++) {
			cairo_move_to(cr, g.x[i], g.y[i]);
			cairo_line_to(cr, g.x[j], g.y[j]);
		}

	g.count = 0;
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
		g.x[g.count] = event->x;
		g.y[g.count] = event->y;
		g.count++;
	}

	if(event->button == 3)
		gtk_widget_queue_draw(widget);

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
