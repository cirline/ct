#define pr_fmt(fmt)	"event   ] " fmt

#include <errno.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>

#include "stkmon/stkmon.h"

int event_monitor(GtkWidget *widget, GdkEvent *event, gpointer pointer)
{
	return 0;
}

int event_update_net_data(struct golden_eye *ss /*gpointer *p*/)
{
	int rc;
	//struct golden_eye *ss = (struct golden_eye *)p;

	if(!ss->pull_index_data) {
		pr_err("%s, no found pull_index_data\n", __func__);
		return - ENODATA;
	}

	rc = ss->pull_index_data(ss);
	if(rc < 0) {
		pr_err("%s, pull index fail, rc = %d\n", __func__, rc);
		return rc;
	}

//	calc_index_realtime_info(ss);
//	calc_print_info(ss);

	return 0;
}


