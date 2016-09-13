#include "ccutils/net.h"
#include "ccutils/log.h"

int main(void)
{
	char buffer[4096];
	pr_info("hello world\n");
	http_get("hq.sinajs.cn/list=sh601766", buffer, 4096);
	printf("result:\n%s", buffer);
	
	return 0;
}
