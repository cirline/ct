#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

double arr[] = {
	0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1,
	0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1
};
int arr_index = 0;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	int i;

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	gint x = width / 2;
	gint y = height / 2;

	cairo_set_line_width(cr, 4);

	cairo_set_source_rgb(cr, 0, 0, 0.2);
	cairo_paint(cr);	// filled background

	cairo_translate(cr, x, y);

	if(arr_index >= 8)
		arr_index = 0;

	for(i = 0; i < 8; i++) {
		cairo_set_source_rgba(cr, 1, 0, 0, arr[arr_index + i]);

		cairo_move_to(cr, 0, -10);
		cairo_line_to(cr, 0, -40);
		cairo_rotate(cr, M_PI / 4);

		cairo_stroke(cr);
	}

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

