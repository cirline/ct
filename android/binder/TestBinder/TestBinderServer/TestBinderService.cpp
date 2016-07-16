#define LOG_TAG "ccc TestBinderService"

#include <cutils/log.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "TestBinderService.h"
static int debug_flag = 1;
namespace android {

void TestBinderService::instantiate() {
	ALOGI("Enter TestBinderService::instantiate");
	status_t st = defaultServiceManager()->addService(
			String16("my.test.binder"), new TestBinderService());
	ALOGD("ServiceManager addService ret=%d", st);
	ALOGD("instantiate> end");
}

TestBinderService::TestBinderService() {
	ALOGD(" TestBinderServicet");
}

TestBinderService::~TestBinderService() {
	ALOGD("TestBinderService destroyed,never destroy normally");
}

int TestBinderService::add(int a,int b) {

	ALOGI("TestBinderService::add a = %d, b = %d.", a , b);	
	return a+b;
}

}
