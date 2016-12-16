#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#define fd_check()	do if (logfp <= 0) logfp = stdout; while(0)

static FILE *logfp;

FILE *pr_init(const char *path)
{
	if(path) {
		logfp = fopen(path, "a+");
	} else
		logfp = stdout;

	return logfp;
}

int pr_log(int prio, const char *tag, const char *s, ...)
{
	va_list args;

	fd_check();
	va_start(args, s);
#ifndef ANDROID
	vfprintf(logfp, s, args);
#else
	__android_log_vprint(prio, tag, s, args);
#endif
	va_end(args);
	return 0;
}

int pr_s2hex(const char *s, ...)
{
	va_list args;
	char buf[4096];
	int len;
	int i, j;

	fd_check();
	va_start(args, s);
	vsprintf(buf, s, args);
	va_end(args);

	len = strlen(buf);
	for(i = 0; i < len;) {
		for(j = 0; j < 16 && i < len; j++) {
			pr_log(0, NULL, "%x ", buf[i++]);
		}
		pr_log(0, NULL, "\n");
	}

	return 0;
}

int pr_level(int level, char symbol, const char *format, ...)
{
	int i;
	va_list args;
	int rc;

	for(i = 0; i < level; i++)
		pr_log(0, NULL, "%c", symbol);

	fd_check();
	va_start(args, format);
#ifndef ANDROID
	rc = vfprintf(logfp, format, args);
#else
	rc = __android_log_vprint(0, NULL, format, args);
#endif
	va_end(args);

	return rc;
}

