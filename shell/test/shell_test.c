#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
	int ret;
	int index;
	const char *opstring = "";

	struct option options[] = {
		/* 0 */
		{"quit", no_argument, 0, 0},
		{"help", no_argument, 0, 0},
		{"fl", no_argument, 0, 0},
		{"fc", no_argument, 0, 0},
		{"fu", no_argument, 0, 0},
		{"fg", no_argument, 0, 0},
		{"fd", no_argument, 0, 0},
		{0,0,0,0}
	};

	printf("hello world.\n");

	while(-1 != (ret = getopt_long_only(argc, argv, opstring, options, &index))){
		printf("index = %d\n", index);
		switch(index){
			case 0:
				printf("case 0\n");
				break;
			case 1:
				printf("case 1\n");
				break;
			default:
				printf("case default\n");
				break;
		}
	}

	return 0;
}

