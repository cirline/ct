#ifndef __UTILS_PTHREAD_H_
#define __UTILS_PTHREAD_H_

#include <pthread.h>

#ifdef ANDROID
#include <jni.h>
#endif

#define thread_desc	pthread_desc

struct pthread_desc {
	int en;
	pthread_t id;
	int what;
	void *p;

	pthread_mutex_t mutex;
	pthread_cond_t cond;
#ifdef ANDROID
	JavaVM *vm;
	JNIEnv *env;
	jclass *cls;
	jobject *obj;
#endif
};

#endif
