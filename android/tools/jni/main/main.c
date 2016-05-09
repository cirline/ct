#define DEBUG
#define pr_fmt(fmt)	"ToolsNative"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#include <jni.h>

#include "../utils/header.h"
#include "header.h"

struct thread_desc *g_mloop;

static void main_loop_cleanup_handler(void *arg)
{
	struct thread_desc *p = arg;
	JavaVM *vm = p->vm;

	pr_info("%s\n", __func__);
	pthread_cond_destroy(&p->cond);
	pthread_mutex_destroy(&p->mutex);
	(*p->env)->DeleteGlobalRef(p->env, p->cls);
	free(p);
	g_mloop = NULL;
	(*vm)->DetachCurrentThread(vm);
}

void * native_main_loop(void *arg)
{
	struct thread_desc *p = arg;

	pthread_cleanup_push(main_loop_cleanup_handler, p);

	(*p->vm)->AttachCurrentThread(p->vm, &p->env, NULL);

	while(p->en) {
		pr_debug("a main loop action:\n");
		pthread_mutex_lock(&p->mutex);
		while(p->what == MSG_THREAD_NONE) {
			pthread_cond_wait(&p->cond, &p->mutex);
		}
		switch(p->what) {
			case MSG_THREAD_EXIT:
				pr_warn("receive thread exit singal.\n");
				p->en = 0;
				break;
			default:
				pr_err("unknown message.\n");
		}
		p->what = MSG_THREAD_NONE;
		pthread_mutex_unlock(&p->mutex);
		sleep(1);
	}

out:
	pthread_cleanup_pop(1);
	pr_warn("native main loop exit.\n");
	return NULL;
}

void * native_message_handler(void *arg)
{
}

int native_create(JNIEnv *env, jclass cls)
{
	pr_info("%s\n", __func__);
	if(g_mloop) {
		/* test thread or free it */
		pr_warn("test or free thread.\n");
		return -1;
	}

	g_mloop = malloc(sizeof(*g_mloop));
	if(!g_mloop) {
		pr_err("malloc g_mloop failed: %s\n", strerror(errno));
		goto err_m_gm;
	}
	memset(g_mloop, 0, sizeof(*g_mloop));

	(*env)->GetJavaVM(env, &g_mloop->vm);
	g_mloop->cls = (*env)->NewGlobalRef(env, g_mloop->cls);

	pthread_mutex_init(&g_mloop->mutex, NULL);
	pthread_cond_init(&g_mloop->cond, NULL);

	g_mloop->en = 1;

	if(pthread_create(&g_mloop->id, NULL, native_main_loop, g_mloop) != 0) {
		pr_err("pthread_create native_main_loop failed: %s", strerror(errno));
		goto err_pc_nml;
	}

	return 0;

err_pc_nml:
	pthread_cond_destroy(&g_mloop->cond);
	pthread_mutex_destroy(&g_mloop->mutex);
	(*env)->DeleteGlobalRef(env, g_mloop->cls);
	free(g_mloop);
	g_mloop = NULL;
err_m_gm:
	return -1;
}

