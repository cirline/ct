#define pr_fmt(fmt)	"main    ] " fmt

#include <string.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>

#include "config.h"
#include "geye/ge.h"


static void geye_app_activate(GtkApplication *app, struct golden_eye_2 *ge)
{
	extern int old_main(GtkApplication *app, gpointer p);

	old_main(app, ge);

	pr_info("%s end\n", __func__);
}

static int geye_app_start(int argc, char *argv[])
{
	struct golden_eye_2 ge;
	GtkApplication *app;
	int rc;

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

