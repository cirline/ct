#include <cairo.h>

int main(void)
{
	cairo_surface_t *sf;
	cairo_t *cr;

	sf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 390, 60);
	cr = cairo_create(sf);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 40.0);

	cairo_move_to(cr, 10.0, 50.0);
	cairo_show_text(cr, "Hello World.");

	cairo_surface_write_to_png(sf, "image.png");

	cairo_destroy(cr);
	cairo_surface_destroy(sf);

	return 0;
}
