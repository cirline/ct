#define LOG_TAG "ccccccccc"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/log.h>


#include "TestBinderService.h"

using namespace android;

int main(int argc, char** argv)
 {
	
	sp<ProcessState> proc(ProcessState::self());
	sp<IServiceManager> sm = defaultServiceManager();
	ALOGI("TestBinderService before");
	TestBinderService::instantiate();
	ALOGI("TestBinderService End");
	ProcessState::self()->startThreadPool();
	IPCThreadState::self()->joinThreadPool();
	return 0;

}


