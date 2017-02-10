#include <cairo.h>
#include <cairo-pdf.h>

int main(void)
{
	cairo_surface_t *sf;
	cairo_t *cr;

	sf = cairo_pdf_surface_create("file.pdf", 504, 648);
	cr = cairo_create(sf);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 40);

	cairo_move_to(cr, 10, 50);
	cairo_show_text(cr, "Hello pdf!");

	cairo_show_page(cr);

	cairo_move_to(cr, 10, 50);
	cairo_show_text(cr, "Hello pdf.");
	cairo_show_page(cr);

	cairo_surface_destroy(sf);
	cairo_destroy(cr);

	return 0;
}
