#define pr_fmt(fmt)	"ToolsJNI"

#include <errno.h>
#include <pthread.h>
#include <android/log.h>

#include "com_chenqiwei_tools_Jni.h"

#include "../utils/header.h"
#include "header.h"

#define LOG_TAG		"ToolsJNI"

#define TOOLS_JNI_VERSION	"0.2"

#define ALOGE(format, args...)	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, format, ##args)

static int g_loop_en = 0;
static JavaVM *g_vm;
static jclass *g_cls;

// ***
extern struct thread_desc * g_mloop;

JNIEXPORT jstring JNICALL Java_com_chenqiwei_tools_Jni_version(JNIEnv *env, jclass cls)
{
	return (*env)->NewStringUTF(env, TOOLS_JNI_VERSION);
}

void * jni_loop(void *arg)
{
	JNIEnv *env;
	jclass jcls;
	jmethodID cb;
	int count = 0;

	pr_info("jni loop in");
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
	pr_err("3 jni loop exit");

	return NULL;
}

JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_enable(JNIEnv *env, jclass cls, jboolean enable)
{
	int rc;

	if(enable) {
		rc = native_create(env, cls);
		if(rc < 0)
			pr_err("native_create failed.\n");
		return rc;
	} else {
		if(!g_mloop) {
			pr_err("native main loop not running.\n");
			return -1;
		}

		g_mloop->what = MSG_THREAD_EXIT;
		pthread_cond_signal(&g_mloop->cond);
	}
	pr_info("native start!");

	return 0;
}

JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_exec
  (JNIEnv *env, jclass cls, jstring s)
{
	char *sp;
}

