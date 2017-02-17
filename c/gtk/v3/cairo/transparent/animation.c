#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

struct {
	gboolean timer;
	gdouble alpha;
	gdouble size;
} g;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	int i;

	cairo_text_extents_t extents;

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	gint x = width / 2;
	gint y = height / 2;

	cairo_set_source_rgb(cr, 0, 0, 0.2);
	cairo_paint(cr);	// filled background

	//cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

	g.size += 0.8;

	if(g.size > 20)
		g.alpha -= 0.01;

	cairo_set_font_size(cr, g.size);
	cairo_set_source_rgb(cr, 1, 1, 1);

	cairo_text_extents(cr, "ZetCode", &extents);	// get the text infomations

	cairo_move_to(cr, x - extents.width / 2 , y);
	cairo_text_path(cr, "ZetCode");		// get a path by text
	cairo_clip(cr);		// select the path (restrict by path)

	cairo_paint_with_alpha(cr, g.alpha);	// set the path content

	if(g.alpha <= 0)
		g.timer = FALSE;
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(!g.timer)
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

	gtk_init(&argc, &argv);

	g.timer = TRUE;
	g.alpha = 1;
	g.size = 1;

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
