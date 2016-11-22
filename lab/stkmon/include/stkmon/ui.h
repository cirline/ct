#ifndef __STKMON_UI_H__
#define __STKMON_UI_H__

#include <gtk/gtk.h>

#include "stkmon/stkmon.h"

int ui_show_dialog_info(GtkWidget *widget, GtkWidget *win, char *msg);
GtkWidget *ui_monitor_create_info_panel(struct sm_xmlcfg *smxc);
GtkWidget *ui_monitor_create_dynamic_table(struct stk_xmlcfg *sxc);

#endif
