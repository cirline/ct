#ifndef __TOOLS_NATIVE_HEADER_H__
#define __TOOLS_NATIVE_HEADER_H__

#include <jni.h>

#include "config.h"

enum native_message {
	MSG_THREAD_NONE = 0,
	MSG_THREAD_EXIT,
	MSG_THREAD_FILESYNC,
};

extern int native_create(JNIEnv *env, jclass cls);
extern int native_send_message(int msg);

extern int fsscan(void);
#endif
