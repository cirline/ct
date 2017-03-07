#include <string.h>

#include <ccutils/log.h>

#include "config.h"

int main(int argc, char *argv[])
{
	extern int old_main(int argc, char *argv[]);

	if(argc > 1) {
		if(strcmp(argv[1], "-v") == 0)
			pr_pkg();
	} else {
		old_main(argc, argv);
	}

	return 0;
}

