#ifndef __CCUTILS_MARCRO_H__
#define __CCUTILS_MARCRO_H__

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GUNC_MINOR__ >= 5)
#define __CU_DEPRECATED_FOR(f)	__attribute((__deprecated__("use '" #f "' instead")))
#else
#define __CU_DEPRECATED_FOR(f)
#endif

#ifdef CU_DISABLE_DEPRECATED_WARNINGS
#define CU_DEPRECATED_FOR(f)	extern
#else
#define CU_DEPRECATED_FOR(f)	__CU_DEPRECATED_FOR(f) extern
#endif

#endif
