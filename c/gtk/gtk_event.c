#include <gtk/gtk.h>

void configure_callback(GtkWindow *win, GdkEvent *event, gpointer *data)
{
	int x, y;
	GString *buf;

	x = event->configure.x;
	y = event->configure.y;

	buf = g_string_new(NULL);
	g_string_printf(buf, "%d, %d", x, y);
	gtk_window_set_title(win, buf->str);

	g_string_free(buf, TRUE);
}

void enter_button_callback(GtkWidget *widget, gpointer *data)
{
	GdkColor color = {0, 27000, 30000, 35000};

	g_printf("enter_button_callback\n");

	gtk_widget_modify_bg(widget, GTK_STATE_PRELIGHT, &color);
}

void button_clicked_callback(GtkWidget *widget, gpointer *data)
{
	g_printf("%s\n", __func__);
}

void toggle_connect_callback(GtkWidget *widget, gpointer *data)
{
	static gint handled_id = 0;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
		g_printf("connect\n");
		handled_id = g_signal_connect(G_OBJECT(data), "clicked", G_CALLBACK(button_clicked_callback), NULL);
	} else {
		g_printf("disconnect\n");
		g_signal_handler_disconnect(data, handled_id);
	}
}

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge)
{
	g_printf("%s\n", __func__);
	if(event->type == GDK_BUTTON_PRESS) {
		if(event->button == 1) {
			gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
					event->button, event->x_root, event->y_root, event->time);
		}
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	g_printf("gtk event!\n");

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 300, 200);
	gtk_window_set_decorated(GTK_WINDOW(win), FALSE);

	GtkWidget *vbox = gtk_vbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	GtkWidget *hbox1 = gtk_hbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(vbox), hbox1);

	GtkWidget *btn1 = gtk_button_new_with_label("Button1");
	gtk_widget_set_size_request(btn1, 100, 30);
	gtk_box_pack_start(GTK_BOX(hbox1), btn1, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(btn1), "enter", G_CALLBACK(enter_button_callback), NULL);

	/* disconnecting a callback */
	GtkWidget *cbox1 = gtk_check_button_new_with_label("Connect");
	gtk_box_pack_start(GTK_BOX(hbox1), cbox1, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(cbox1), "clicked", G_CALLBACK(toggle_connect_callback), btn1);

	GtkWidget *hbox2 = gtk_hbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(vbox), hbox2);

	/* a timer */
	gtk_widget_add_events(GTK_WIDGET(win), GDK_CONFIGURE);
	g_signal_connect(G_OBJECT(win), "configure-event", G_CALLBACK(configure_callback), NULL);

	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);
	g_signal_connect(G_OBJECT(win), "button-press-event", G_CALLBACK(on_button_press), NULL);

	g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(win));

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}

