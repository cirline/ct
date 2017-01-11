#ifndef __GOLDEN_EYE_GE_H__
#define __GOLDEN_EYE_GE_H__

#include <gtk/gtk.h>
#include "stkmon/stkmon.h"

#define COLOR_RISE	"black"		// red
#define COLOR_STOPP	"black"		// red

struct golden_eye_2 {
	char		version[32];

	struct golden_eye	old;
};

#endif
