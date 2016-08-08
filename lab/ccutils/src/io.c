//#define DEBUG

#include <stdio.h>
#include <string.h>

#include <ccutils/log.h>

void io_getdata(const char *tip, const char *fmt, int slen, void *p)
{
	int rc;
	int i;
	char input[1024];

	while(1) {
		printf("%s", tip);
		for(i = 0; (input[i] = getchar()) != '\n'; i++)
			;
		input[i] = 0;
		//pr_info("input: %s\n", input);
		if(i > 0) {
			if(!strcmp(input, "...")) {
				break;
			} else if(slen == 0 || (slen > 0 && strlen(input) == slen)) {
				sscanf(input, fmt, p);
				break;
			}
		}
	}
}

