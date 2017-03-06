#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;
cairo_surface_t *g_sf;
gint gw, gh;
gboolean gtimer = TRUE;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	static gint count = 0;

	cairo_t *ic = cairo_create(g_sf);

	gint i, j;
	for(i = 0; i < gh; i += 7) {
		for(j = 0; j < count; j++) {
			cairo_move_to(ic, 0, j + i);
			cairo_line_to(ic, gw, j + i);
		}
	}

	count++;
	if(count == 8)
		gtimer = FALSE;

	cairo_set_source_surface(cr, g_image, 10, 10);
	cairo_mask_surface(cr, g_sf, 10, 10);
	cairo_stroke(ic);

	cairo_destroy(ic);
}

static gboolean timer_handler(GtkWidget *widget)
{
	if(!gtimer)
		return FALSE;

	gtk_widget_queue_draw(widget);

	return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer p)
{
	do_drawing(widget, cr);

	return FALSE;
}

static void draw_mark(void)
{
	cairo_t *wm = cairo_create(g_image);
	cairo_set_font_size(wm, 12);

	cairo_set_source_rgb(wm, 0.9, 0.9, 0.9);
	cairo_move_to(wm, 20, 30);
	cairo_show_text(wm, "Chen Qi Wii");
	cairo_stroke(wm);
}

int main(int argc, char *argv[])
{
	GtkWidget *win;
	GtkWidget *darea;

	g_image = cairo_image_surface_create_from_png("demo.png");
	draw_mark();

	gw = cairo_image_surface_get_width(g_image);
	gh = cairo_image_surface_get_height(g_image);

	g_sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, gw, gh);

	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(win), darea);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);

	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_timeout_add(500, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

