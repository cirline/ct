#ifndef __GOLDEN_EYE_GE_H__
#define __GOLDEN_EYE_GE_H__

#include <gtk/gtk.h>
#include "stkmon/stkmon.h"

#define COLOR_RISE	"black"		// red
#define COLOR_STOPP	"black"		// red

struct ge_mainui {
	int		width;
	int		height;
	GtkWidget	*monitor_dynamic;
};

struct golden_eye_2 {
	char		version[32];

	struct ge_mainui	ui;

	struct golden_eye	old;
};

#endif
