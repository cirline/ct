#include <ccutils/net.h>
#include <ccutils/log.h>

#include "sinajs.h"

int main(int argc, char *argv[])
{
	char buffer[4096];
	char *p;
	struct sinajs data;

	http_get("hq.sinajs.cn/list=sh601766,sh601398", buffer, 4096);

	pr_info("result:\n%s", buffer);

	p = split_http_response_header(buffer);

	pr_info("content:\n%s", p);

	sscanf(p, HTTP_FORMAT_STRING, HTTP_ARGS_LIST(&data));

	pr_info("code = %s, date = %s\n", data.code, data.date);

	return 0;
}
