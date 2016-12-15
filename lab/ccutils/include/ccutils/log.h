#ifndef __CT_UTILS_LOG_H__
#define __CT_UTILS_LOG_H__

#include <stdio.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#define ARRAY_SIZE(arr)		(sizeof(arr) / sizeof(arr[0]))

enum __e_debug_level {
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_ALL,
};

#ifndef pr_fmt
#define pr_fmt(fmt)	fmt
#endif

#ifdef DEBUG
#ifdef ANDROID
#define pr_debug(s, ...)	pr_log(ANDROID_LOG_INFO, pr_fmt(""), s, ##__VA_ARGS__)
#else
#define pr_debug(s, ...)	pr_log(LOG_LEVEL_DEBUG, NULL, "D/"pr_fmt(s), ##__VA_ARGS__)
#endif
#else
#define pr_debug(s, ...)
#endif

#ifdef ANDROID
#define pr_info(s, ...)		pr_log(ANDROID_LOG_INFO, pr_fmt(""), s, ##__VA_ARGS__)
#define pr_warn(s, ...)		pr_log(ANDROID_LOG_WARN, pr_fmt(""), s, ##__VA_ARGS__)
#define pr_err(s, ...)		pr_log(ANDROID_LOG_ERROR, pr_fmt(""), s, ##__VA_ARGS__)
#else
#define pr_info(s, ...)		pr_log(LOG_LEVEL_INFO, NULL, "I/"pr_fmt(s), ##__VA_ARGS__)
#define pr_warn(s, ...)		pr_log(LOG_LEVEL_WARN, NULL, "W/"pr_fmt(s), ##__VA_ARGS__)
#define pr_err(s, ...)		pr_log(LOG_LEVEL_ERROR, NULL, "E/"pr_fmt(s), ##__VA_ARGS__)
#endif

#define pr_enter()		pr_debug("enter, %s, %d\n", __func__, __LINE__)
#define pr_exit()		pr_debug("exit, %s, %d\n", __func__, __LINE__)
#define pr_here()		pr_info("current, %s, %d\n", __func__, __LINE__)

#define pr_pkg()		pr_info("~app: %s ~bugs-report: %s\n", PACKAGE_STRING, PACKAGE_BUGREPORT)

extern int pr_init(const char *path);
extern int pr_log(int prio, const char * tag, const char *s, ...);
int pr_s2hex(const char *s, ...);
int pr_level(int level, char symbol, const char *format, ...);

#endif
