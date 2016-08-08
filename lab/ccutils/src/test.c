
#define pr_fmt(fmt) "test " fmt

#include "log.h"
#include "ustring.h"

int main(void)
{
	int i;
	char ext[16];
	char filename[] = "我的朋友.mp3";

	//pr_init("test.tmp.log");

	pr_debug("debug message.\n");
	pr_info("%s message\n", "info");
	pr_err("error %d message\n", 99);

	pr_s2hex("hello %s\n", "s2hex");

	utf8to16("我的朋友.mp3");
	i = utf8rchr(filename, '.');

	pr_info(". index = %d\n", i);

	pr_info("get ext: %s\n", utf8_get_file_extension(ext, filename));
	
	return 0;
}
