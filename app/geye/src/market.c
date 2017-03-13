#define pr_fmt(fmt)	"market  ] " fmt

#include <gtk/gtk.h>

static int market_netdata_update(struct golden_eye_2 *ge)
{
	int rc;

	if(event_netdata_update(ge) < 0) {
		pr_err("netdata update fail\n");
		return - EINVAL;
	}

	calc_readtime_info(ge);
	calc_pr_info(ge);

	return 0;
}

static gboolean market_net_request(struct golden_eye_2 *ge)
{
	int rc;
	
	rc = market_netdata_update(ge);
	if(rc < 0)
		return TRUE;

	market_display_update(ge);

	return TRUE;
}

void market_ui_start(GtkApplication *app, struct golden_eye_2 *ge)
{
	GtkBuilder *builder;
	GtkWidget *win;

	pr_info("%s\n", __func__);

	builder = gtk_builder_new_from_file("layout/market_list.glade");
	win = GTK_WIDGET(gtk_builder_get_object(builder, "market_list"));
	gtk_application_add_window(app, GTK_WINDOW(win));

	ge->ui.market = win;
	g_object_set_data(G_OBJECT(win), "mdata", ge);
	g_object_set_data(G_OBJECT(win), "builder", builder);

	g_timeout_add(5500, market_net_request, ge);
	gtk_builder_connect_signals(builder, NULL);

	g_object_set_data(G_OBJECT(win), "builder", NULL);
	g_object_unref(G_OBJECT(builder));

	gtk_widget_show_all(win);

	market_net_request(ge);
}

