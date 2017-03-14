#define pr_fmt(fmt)	"event   ] " fmt

#include <errno.h>
#include <gtk/gtk.h>

#include <ccutils/log.h>

#include "geye/ge.h"

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

	return 0;
}

int event_netdata_update(struct golden_eye_2 *ge)
{
	if(!ge->pull_index_data) {
		pr_warn("%s, pull_index_data null\n", __func__);
	} else {
		ge->pull_index_data(ge);
	}

	if(!ge->pull_stock_data)
		pr_warn("%s, pull_stock_data null\n", __func__);
	else
		ge->pull_stock_data(ge);

	return 0;
}


