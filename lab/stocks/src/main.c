#define pr_fmt(fmt)	"main: " fmt
#include <ccutils/log.h>

int main(void)
{
	pr_info("main start ...\n");
	return 0;
}

