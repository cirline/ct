#ifndef __TOOLS_NATIVE_HEADER_H__
#define __TOOLS_NATIVE_HEADER_H__

#include <jni.h>

enum _thread_message {
	MSG_THREAD_NONE = 0,
	MSG_THREAD_EXIT, 
};

extern int native_create(JNIEnv *env, jclass cls);
#endif
