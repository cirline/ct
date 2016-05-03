#ifndef __STRING_UTF8_H__
#define __STRING_UTF8_H__

enum utf8_char {
	UTF8_C1,
	UTF8_C2,
	UTF8_C3,

	UTF8_CSUB,
	UTF8_UNKNOWN,
};

extern char *utf8to16(const char *utf8);
extern int utf8chr_revert(const char * utf8, char c);
extern char * utf8_get_file_extension(char *ext, const char *filename);

#endif
