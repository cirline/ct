#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

cairo_surface_t *g_image;

static void do_drawing(GtkWidget *widget, cairo_t *cr)
{
	static gdouble angle = 0;
	static gdouble scale = 1;
	static gdouble delta = 0.01;

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	gint w, h;
	gtk_window_get_size(GTK_WINDOW(win), &w, &h);

	cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 13);

	cairo_move_to(cr, 20, 30);
	cairo_show_text(cr, "hello line 1");
	cairo_move_to(cr, 20, 60);
	cairo_show_text(cr, "hello line 2");

	/* centered text */
	cairo_text_extents_t extents;
	cairo_text_extents(cr, "hello line 3", &extents);
	cairo_move_to(cr, w / 2 - extents.width / 2, 90);
	cairo_show_text(cr, "heloo line 3");

	/* shaded text */
	cairo_move_to(cr, 20, 120);
	cairo_show_text(cr, "hello line 4");

	cairo_set_source_rgb(cr, 0, 0, 1);
	cairo_move_to(cr, 21, 121);
	cairo_show_text(cr, "hello line 4");
}

static gboolean timer_handler(GtkWidget *widget)
{
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

	g_timeout_add(200, (GSourceFunc)timer_handler, (gpointer)win);

	gtk_widget_show_all(win);

	gtk_main();

	cairo_surface_destroy(g_image);

	return 0;
}

