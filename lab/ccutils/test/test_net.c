#include <string.h>
#include <curl/curl.h>

#include "ccutils/net.h"
#include "ccutils/log.h"

int main(void)
{
	char buffer[4096];
	CURL *curl;
	CURLcode res;

#if 0
	pr_info("hello world\n");
	http_get("hq.sinajs.cn/list=sh601766", buffer, 4096);
	printf("result:\n%s", buffer);

	printf("curl test:\n");
#endif

#if 0
	curl = curl_easy_init();
	if(curl) {
		//curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		curl_easy_setopt(curl, CURLOPT_URL, "http://hq.sinajs.cn/list=sh601766");
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			printf("curl_easy_perform failed: %s\n", curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl);
	}
#endif

	memset(buffer, 0, 4096);
	http_get("hq.sinajs.cn/list=sh601766", buffer);
	printf("%s\nsize = %d\n", buffer, strlen(buffer));

	return 0;
}
