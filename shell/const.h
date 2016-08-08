#ifndef __UINFO_CONST__
#define __UINFO_CONST__


#define DESC_MAX_LEN	64
#define FIELD_MAX_LEN	64

#define CMD_MAX_NUM		16
#define CMD_MAX_LEN		64

/* all commands */
typedef enum {
	CMD_QUIT = 0,
	CMD_HELP,
	/* feild command */
	CMD_FIELD_LIST,
	CMD_FIELD_CREATE,
	CMD_FIELD_UPDATE,
	CMD_FIELD_GEN,
	CMD_FIELD_DELETE,
	/* commands end*/
	END_CMDLIST
} cmd_t;

#endif
