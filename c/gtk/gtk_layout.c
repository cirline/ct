#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 300, 200);
#if 0
	GtkWidget *fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(win), fixed);

	GtkWidget *btn1 = gtk_button_new_with_label("B1");
	gtk_fixed_put(GTK_FIXED(fixed), btn1, 150, 50);
	gtk_widget_set_size_request(btn1, 80, 30);

	GtkWidget *btn2 = gtk_button_new_with_label("B2");
	gtk_fixed_put(GTK_FIXED(fixed), btn2, 30, 30);
	gtk_widget_set_size_request(btn2, 80, 30);
#elif 0
	GtkWidget *align = gtk_alignment_new(0, 1, 0, 0);
	GtkWidget *lbl = gtk_label_new("bottom-left");

	gtk_container_add(GTK_CONTAINER(align), lbl);
	gtk_container_add(GTK_CONTAINER(win), align);
#elif 0
	/* vbox */
	GtkWidget *vbox = gtk_vbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	GtkWidget *settings = gtk_button_new_with_label("Settings");
	GtkWidget *accounts = gtk_button_new_with_label("Accounts");

	gtk_box_pack_start(GTK_BOX(vbox), settings, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), accounts, TRUE, TRUE, 0);
#elif 0
	/* table */
	char *values[16] = {
		"7", "8", "9", "/",
		"4", "5", "6", "*",
		"1", "2", "3", "-",
		"0", ".", "=", "+",
	};

	GtkWidget *tbl = gtk_table_new(4, 4, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(tbl), 2);
	gtk_table_set_col_spacings(GTK_TABLE(tbl), 2);

	int i, j, pos;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++) {
			GtkWidget *button = gtk_button_new_with_label(values[pos]);
			gtk_table_attach_defaults(GTK_TABLE(tbl), button, j, j+1, i, i+1);
			pos++;
		}

	gtk_container_add(GTK_CONTAINER(win), tbl);
#elif 0
	/* corner buttons */
	GtkWidget *vbox = gtk_vbox_new(FALSE, 5);
	GtkWidget *valign = gtk_alignment_new(0, 1, 0, 0);	// button align
	gtk_container_add(GTK_CONTAINER(vbox), valign); // put valign into vbox
	gtk_container_add(GTK_CONTAINER(win), vbox);	// put vbox into win

	GtkWidget *hbox = gtk_hbox_new(TRUE, 3);	// homogeneous = TRUE, set all child widget same
	GtkWidget *btn_ok = gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(btn_ok, 70, 30);
	gtk_container_add(GTK_CONTAINER(hbox), btn_ok);
	GtkWidget *btn_close = gtk_button_new_with_label("Close");
	gtk_container_add(GTK_CONTAINER(hbox), btn_close);

	GtkWidget *halign = gtk_alignment_new(1, 0, 0, 0);	// right align
	gtk_container_add(GTK_CONTAINER(halign), hbox);		// put hbox into halign

	gtk_box_pack_start(GTK_BOX(vbox), halign, FALSE, FALSE, 0);	// pack the halign into vbox
	/* the alignment container can take only one child widget */
#elif 1
	GtkWidget *table = gtk_table_new(6, 4, FALSE);		// rows = 6, cols = 4
	gtk_table_set_col_spacings(GTK_TABLE(table), 3);
	gtk_table_set_row_spacing(GTK_TABLE(table), 0, 3);

	GtkWidget *title = gtk_label_new("Windows");
	GtkWidget *halign = gtk_alignment_new(0, 0, 0, 0);	// top, left
	gtk_container_add(GTK_CONTAINER(halign), title);	// put title into halign
	gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);	// left = top = 0, right = buttom = 1

	GtkWidget *wins = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(wins), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(wins), FALSE);
	gtk_table_attach(GTK_TABLE(table), wins, 0, 2, 1, 3, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);	// left = 0, right = 2, top = 1, buttom = 3

	GtkWidget *btn_act = gtk_button_new_with_label("Activate");
	gtk_widget_set_size_request(btn_act, 100, 30);
	gtk_table_attach(GTK_TABLE(table), btn_act, 3, 4, 1, 2, GTK_FILL, GTK_SHRINK, 1, 1);	// left = 3, right = 4, top = 1, buttom = 2

	GtkWidget *valign = gtk_alignment_new(0, 0, 0, 0);	// top, left
	GtkWidget *btn_close = gtk_button_new_with_label("Close");
	gtk_widget_set_size_request(btn_close, 100, 30);
	gtk_container_add(GTK_CONTAINER(valign), btn_close);	// put btn_close into valign
	gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
	gtk_table_attach(GTK_TABLE(table), valign, 3, 4, 2, 3, GTK_FILL, GTK_FILL | GTK_EXPAND, 1, 1);	// (3, 2) -> (4, 3)

	GtkWidget *halign2 = gtk_alignment_new(0, 1, 0, 0);	// left, buttom
	GtkWidget *btn_help = gtk_button_new_with_label("Help");
	gtk_container_add(GTK_CONTAINER(halign2), btn_help);	// put btn_help into halign2
	gtk_widget_set_size_request(btn_help, 100, 30);
	gtk_table_set_row_spacing(GTK_TABLE(table), 3, 5);
	gtk_table_attach(GTK_TABLE(table), halign2, 0, 1, 4, 5, GTK_FILL, GTK_FILL, 0, 0);	// (0, 4) -> (1, 5)

	GtkWidget *btn_ok = gtk_button_new_with_label("OK");
	gtk_widget_set_size_request(btn_ok, 100, 30);
	gtk_table_attach(GTK_TABLE(table), btn_ok, 3, 4, 4, 5, GTK_FILL, GTK_FILL, 0, 0);	// (3, 4) -> (4, 5)

	gtk_container_add(GTK_CONTAINER(win), table);
#endif

	g_signal_connect(GTK_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

