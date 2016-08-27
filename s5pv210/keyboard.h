#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

/* keyboard defined */
typedef enum kb_def {
	KB_UP	= 0,
	KB_DOWN,
	KB_LEFT,
	KB_RIGHT,
	KB_ENTER,
	KB_BACK,
	KB_HOME,
	KB_POWER,
	KB_UNKNOWN,
} kb_def_t;

/* key description */
#define KB_DESC_UP		"up"
#define KB_DESC_DOWN	"down"
#define KB_DESC_LEFT	"left"
#define KB_DESC_RIGHT	"right"
#define KB_DESC_ENTER	"enter"
#define KB_DESC_BACK	"back"
#define KB_DESC_HOME	"home"
#define KB_DESC_POWER	"power"
#define KB_DESC_UNKNOWN	"unknown"

/* key press interrupt function */
extern int keydown(void);

#endif
