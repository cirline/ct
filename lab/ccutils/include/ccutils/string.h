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
extern int utf8rchr(const char * utf8, char c);
extern char * utf8_get_file_extension(char *ext, const char *filename);
extern unsigned int strhash(const char *s, unsigned int len);
extern size_t convert_charset(
		const char *out_charset,
		const char *in_charset,
		char *outb,
		size_t outb_size,
		char *inb,
		size_t inb_size);

#endif
