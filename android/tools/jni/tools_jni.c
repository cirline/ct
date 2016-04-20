#include <errno.h>
#include <pthread.h>
#include <android/log.h>

#include "com_chenqiwei_tools_Jni.h"

#define LOG_TAG		"ToolsJNI"

#define ALOGI(format, args...)	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, format, ##args)
#define ALOGE(format, args...)	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, format, ##args)

static int g_loop_en = 0;
static JavaVM *g_vm;
static jclass *g_cls;

JNIEXPORT jstring JNICALL Java_com_chenqiwei_tools_Jni_version(JNIEnv *env, jclass cls)
{
	return (*env)->NewStringUTF(env, "0.1");
}

void * jni_loop(void *arg)
{
	JNIEnv *env;
	jclass jcls;
	jmethodID cb;
	int count = 0;

	ALOGI("jni loop in");
	(*g_vm)->AttachCurrentThread(g_vm, &env, NULL);

	//cb = (*env)->GetMethodID(env, g_cls, "toString", "(Ljava/lang/String;)V");
	cb = (*env)->GetStaticMethodID(env, g_cls, "toString", "(I)V");
	if(cb == NULL) {
		ALOGE("get method id failed.");
		goto out;
	}

	while(g_loop_en) {
		(*env)->CallStaticVoidMethod(env, g_cls, cb, count++);
		sleep(1);
	}

out:
	(*g_vm)->DetachCurrentThread(g_vm);
	ALOGI("jni loop exit");

	return NULL;
}

JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_enable(JNIEnv *env, jclass cls, jboolean enable)
{
	pthread_t id;

	if(enable == JNI_FALSE) {
		g_loop_en = 0;
		return 0;
	}

	g_loop_en = 1;

	(*env)->GetJavaVM(env, &g_vm);
	g_cls = (*env)->NewGlobalRef(env, cls);

#if 0
	{
	jmethodID cb;
	cb = (*env)->GetStaticMethodID(env, cls, "toString", "(I)V");
	if(cb == NULL)
		ALOGE("start get method failed.");
	else {
		ALOGI("start call method. 1");
		(*env)->CallStaticVoidMethod(env, cls, cb, 93);
	}
	}
#else
	if(pthread_create(&id, NULL, jni_loop, NULL) != 0) {
		ALOGE("pthread_create failed, %s", strerror(errno));
		return -1;
	}
#endif
	ALOGI("start finish!");
	return 0;
}

