#define pr_fmt(fmt)	"string_utf8 " fmt

#include <string.h>
#include "ccutils/log.h"
#include "ccutils/ustring.h"

enum utf8_char utf8_char_check(char c)
{
	if((c & 0x80) == 0)
		return UTF8_C1;
	else if((c & 0xc0) == 0x80)
		return UTF8_CSUB;
	else if((c & 0xe0) == 0xc0)
		return UTF8_C2;
	else if((c & 0xf0) == 0xe0)
		return UTF8_C3;
	else
		return UTF8_UNKNOWN;
}

char *utf8to16(const char *utf8)
{
	int i;
	int len;

	len = strlen(utf8);
	pr_info("len: %d\n", len);
	for(i = 0; i < len; i++) {
		switch(utf8_char_check(utf8[i])) {
			case UTF8_C1:
				pr_info("c1\n");
				break;
			case UTF8_C2:
				pr_info("c2\n");
				break;
			case UTF8_C3:
				pr_info("c3\n");
				break;
			case UTF8_CSUB:
				pr_info("csub\n");
				break;
			case UTF8_UNKNOWN:
				pr_info("unk\n");
				break;
		}
	}

	return NULL;
}

/**
 * utf8 string search a char
 */

/**
 * utf8 string revert search a char
 */
int utf8rchr(const char * utf8, char c)
{
	int len;
	int i;

	len = strlen(utf8);

	for(i = len - 1; i >= 0; i--) {
		if(utf8_char_check(utf8[i]) == UTF8_C1)
			if(utf8[i] == c)
				break;
	}

	return i;
}

/**
 * get a utf8 file name extension
 */
char * utf8_get_file_extension(char *ext, const char *filename)
{
	int i;

	i = utf8rchr(filename, '.');
	if(i > 0)
		strcpy(ext, filename + i + 1);
	else
		*ext = 0;

	return ext;
}

unsigned int strhash(const char *s, unsigned int len)
{
	unsigned int hash;

	for(hash = 0; *s; s++) {
		hash = (hash * 9) ^ *s;
	}

	return hash % len;
}

