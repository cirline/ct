#define pr_fmt(fmt)	"main    ] " fmt

#include <string.h>
#include <gtk/gtk.h>
#include <sys/queue.h>

#include <ccutils/log.h>
#include "config.h"
#include "geye/ge.h"
#include "geye/market.h"
#include "geye/sinajs.h"
#include "geye/parser.h"

extern void monitor_ui_start(GtkApplication *app, struct golden_eye_2 *ge);

static void geye_app_activate(GtkApplication *app, struct golden_eye_2 *ge)
{
	extern int old_main(GtkApplication *app, gpointer p);

//	old_main(app, ge);

	market_ui_start(app, ge);

	monitor_ui_start(app, ge);

	pr_info("%s end\n", __func__);
}

static int geye_app_data_init(struct golden_eye_2 *ge)
{
	int rc;

	ge->pull_stock_data = NULL;
	ge->pull_index_data = NULL;

	rc = parser_load_xml(ge);
	if(rc < 0) {
		pr_err("load xml fail\n");
		return rc;
	}

	if(ge->index_count > 0)
		ge->pull_index_data = sinajs_pull_index_data_v2;

	if(ge->stock_count > 0)
		ge->pull_stock_data = sinajs_pull_stock_data;

	return 0;
}

static int geye_app_start(int argc, char *argv[])
{
	struct golden_eye_2 ge;
	GtkApplication *app;
	int rc;

	geye_app_data_init(&ge);

	app = gtk_application_new("local.chqw.goldeneye", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(geye_app_activate), &ge);
	rc = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return rc;
}

int main(int argc, char *argv[])
{
	if(argc > 1) {
		if(strcmp(argv[1], "-v") == 0)
			pr_pkg();
	} else {
		geye_app_start(argc, argv);
	}

	return 0;
}

