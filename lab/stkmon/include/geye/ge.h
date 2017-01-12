#ifndef __GOLDEN_EYE_GE_H__
#define __GOLDEN_EYE_GE_H__

#include <gtk/gtk.h>
#include "stkmon/stkmon.h"

#define COLOR_RISE	"#000000"		// red
#define COLOR_STOPP	"#ff0000"		// red
#define COLOR_EQ	"black"
#define COLOR_DROP	"#888888"		//"gray"
#define COLOR_STOPL	"green"

struct ge_mainui {
	int		width;
	int		height;
	GtkWidget	*monitor_dynamic;

	GtkWidget	*win;
};

struct golden_eye_2 {
	char		version[32];

	struct ge_mainui	ui;

	struct golden_eye	old;
};

#endif
