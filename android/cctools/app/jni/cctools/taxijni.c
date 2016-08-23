#include <unistd.h>
#include <hardware/hardware.h>

#define LOG_TAG	"taxijni"
#include <cutils/log.h>

#include <jni.h>
#include "taxihal.h"

#define JAVA_CLASS	"com/szhklt/toolbox/NativeTools"

static struct taxi_device_t *g_dev = NULL;

static jboolean taxi_init(JNIEnv *env, jclass cls)
{
	struct taxi_module_t *module;
	int rc;

	ALOGI("%s", __func__);

	if(g_dev) {
		ALOGW("dev is exist");
		return JNI_TRUE;
	}

	rc = hw_get_module(TAXI_HARDWARE_MODULE_ID, (const struct hw_module_t **)&module);
	if(rc < 0 || !module) {
		ALOGE("hw_get_module %s failed", TAXI_HARDWARE_MODULE_ID);
		return JNI_FALSE;
	}

	module->common.methods->open(&module->common, TAXI_HARDWARE_MODULE_ID, &g_dev);

	return JNI_TRUE;
}

static jboolean native_logsave(JNIEnv *env, jclass cls)
{
	int pid;

	ALOGI("%s", __func__);
#if 0
	if(g_dev)
		g_dev->logsave(g_dev);
#endif

	pid = fork();

	if(pid == 0) {
		ALOGI("c save log\n");
		execlp("logcat", "logcat", "-b", "main", "-r", "512", "-f", "/data/taxid/native_save.log", NULL);
	}
	ALOGI("p return");

	return JNI_TRUE;
}

static JNINativeMethod g_methods[] = {
	{"init_native", "()Z", (void *)taxi_init},
	{"logsave", "()Z", (void *)native_logsave},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	jclass cls;
	jint rc;

	ALOGI("%s", __func__);
	rc = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4);
	if(rc != JNI_OK) {
		ALOGE("JavaVM cannot get env");
		return rc;
	}
	if(!env) {
		ALOGE("env == NULL");
		return JNI_ERR;
	}
	cls = (*env)->FindClass(env, JAVA_CLASS);
	if(!cls) {
		ALOGE("cannot find class %s", JAVA_CLASS);
		return JNI_ERR;
	}
	rc = (*env)->RegisterNatives(env, cls, g_methods, sizeof(g_methods)/sizeof(g_methods[0]));
	if(rc < 0) {
		ALOGE("RegisterNatives failed");
	}

	return rc == 0 ? JNI_VERSION_1_4 : JNI_ERR;
}

void JNI_OnUnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	jclass cls;
	jint rc;

	ALOGI("%s", __func__);
	rc = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4);
	if(rc != JNI_OK) {
		ALOGE("JavaVM cannot get env");
		return;
	}
	if(!env) {
		ALOGE("env == NULL");
		return;
	}
	cls = (*env)->FindClass(env, JAVA_CLASS);
	if(!cls) {
		ALOGE("cannot find class %s", JAVA_CLASS);
		return JNI_ERR;
	}
	rc = (*env)->UnregisterNatives(env, cls);
	if(rc < 0) {
		ALOGE("UnregisterNatives failed");
	}
}
