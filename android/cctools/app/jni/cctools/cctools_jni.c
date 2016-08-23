#include <errno.h>
#include <pthread.h>

#include <jni.h>

#include <ccutils/log.h>

#define TOOLS_JNI_VERSION	"0.2"
#define JAVA_CLASS	"com/chenqiwei/cctools/CTNative"

static int g_loop_en = 0;
static JavaVM *g_vm;
static jclass *g_cls;

extern struct thread_desc * g_mloop;

static jstring native_version(JNIEnv *env, jclass cls)
{
	return (*env)->NewStringUTF(env, TOOLS_JNI_VERSION);
}

#if 0
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
		pr_err("get method id failed\n");
		goto out;
	}

	while(g_loop_en) {
		(*env)->CallStaticVoidMethod(env, g_cls, cb, count++);
		sleep(1);
	}

out:
	(*g_vm)->DetachCurrentThread(g_vm);
	pr_err("3 jni loop exit\n");

	return NULL;
}

JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_enable(JNIEnv *env, jclass cls, jboolean enable)
{
	return enable ? native_create(env, cls) : native_send_message(MSG_THREAD_EXIT);
}

JNIEXPORT jint JNICALL Java_com_chenqiwei_tools_Jni_exec
  (JNIEnv *env, jclass cls, jstring s)
{
	const char *sp;
	int len;

	len = (*env)->GetStringUTFLength(env, s);
	sp = (*env)->GetStringUTFChars(env, s, NULL); // isCopy use to transfer return
	pr_info("string length = %d, context: %s\n", len, sp);
	(*env)->ReleaseStringUTFChars(env, s, sp);

	return 0;
}

JNIEXPORT void JNICALL Java_com_chenqiwei_tools_Jni_fileSync
  (JNIEnv *env, jclass cls)
{
	native_send_message(MSG_THREAD_FILESYNC);
}
#endif

static JNINativeMethod g_methods[] = {
	/* {"init_native", "()Z", (void *)native_init}, */
	{"version", "()Ljava/lang/String;", (void *)native_version},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	jclass cls;
	jint rc;

	pr_info("%s in\n", __func__);
	rc = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4);
	if(rc != JNI_OK) {
		pr_err("JavaVM cannot get env\n");
		return rc;
	}
	if(!env) {
		pr_err("env == NULL\n");
		return JNI_ERR;
	}
	cls = (*env)->FindClass(env, JAVA_CLASS);
	if(!cls) {
		pr_err("cannot find class %s\n", JAVA_CLASS);
		return JNI_ERR;
	}
	rc = (*env)->RegisterNatives(env, cls, g_methods, sizeof(g_methods)/sizeof(g_methods[0]));
	if(rc < 0) {
		pr_err("RegisterNatives failed\n");
	}

	return rc == 0 ? JNI_VERSION_1_4 : JNI_ERR;
}

void JNI_OnUnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	jclass cls;
	jint rc;

	pr_info("%s\n", __func__);
	rc = (*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4);
	if(rc != JNI_OK) {
		pr_err("JavaVM cannot get env\n");
		return;
	}
	if(!env) {
		pr_err("env == NULL\n");
		return;
	}
	cls = (*env)->FindClass(env, JAVA_CLASS);
	if(!cls) {
		pr_err("cannot find class %s\n", JAVA_CLASS);
		return ;
	}
	rc = (*env)->UnregisterNatives(env, cls);
	if(rc < 0) {
		pr_err("UnregisterNatives failed\n");
	}
}
